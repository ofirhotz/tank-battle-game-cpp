#include "theTankBattle.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>

using namespace std;

theTankBattle::theTankBattle()
{
	string filesPrefix("tanks-game");
	string filesPostfix(".screen");
	const filesystem::path currentPath = filesystem::current_path();

	// iterate over the all files in the current directory
	for (const auto& directoryEntry : filesystem::directory_iterator(currentPath))
	{
		// make sure that is it a regule file
		if (directoryEntry.is_regular_file())
		{
			std::string filename = directoryEntry.path().filename().string();

			// make sure that the file name fits
			if (filename.substr(0, filesPrefix.size()) == filesPrefix &&
				filename.substr(filename.length() - filesPostfix.size()) == filesPostfix)
			{
				fileNames.push_back(filename);
			}
		}
	}
	// Sort the files names
	std::sort(fileNames.begin(), fileNames.end());
}

theTankBattle::~theTankBattle() {
	if (stepsOutputFile.is_open()) {
		stepsOutputFile.close();
	}
	if (stepsInputFile.is_open()) {
		stepsInputFile.close();
	}
}

void theTankBattle::displayFileNamesMenu() const
{
	if (!silent) {
		setCursorPosition(0, 0);
		cout << "================ FILES MENU ================" << endl;
		for (int i = 0; i < fileNames.size(); i++)
		{
			cout << (i + 1) << ". " << fileNames[i] << endl;
		}
		cout << "===========================================" << endl;
	}
}

void theTankBattle::displayPlayersTypesMenu() const
{
	if (!silent) {
		setCursorPosition(0, 0);
		cout << "==== SELECT PLAYERS TYPES MENU ====" << endl;
		cout << "1. Human vs. Human" << endl;
		cout << "2. Human vs. Computer" << endl;
		cout << "3. Computer vs. Computer" << endl;
		cout << "===================================" << endl;
	}
}

bool theTankBattle::init()
{
	char userInput = ' ';
	while (true)
	{
		IO_Utils::clear_screen();
		displayFileNamesMenu();
		if (gameMode != REGULAR) {
			userInput = '1';
		}
		else {
			userInput = _getch(); // waits for keypress
		}
		if ((userInput > '0') && (userInput <= ('0' + fileNames.size())))
		{
			break;
		}
		if (!silent) {
			cout << "Bad input." << endl << "Click Enter to continue..." << endl;
		}
		userInput = _getch(); // waits for keypress
	}
	currentFileIndex = userInput - '0' - 1;
	while (true)
	{
		IO_Utils::clear_screen();
		displayPlayersTypesMenu();
		if (gameMode != REGULAR) {
			userInput = '1';
		}
		else {
			userInput = _getch(); // waits for keypress
		}
		if ((userInput > '0') && (userInput <= '3'))
		{
			break;
		}
		if (!silent) {
			cout << "Bad input." << endl << "Click Enter to continue..." << endl;
		}
		userInput = _getch(); // waits for keypress
	}
	mode = userInput - '0';
	return loadNextBoard();
}

bool theTankBattle::loadNextBoard()
{
	if ((currentFileIndex < 0) || (currentFileIndex >= fileNames.size())) {
		return false;
	}
	fillBoard();
	drawBoard();
	return true;
}

void theTankBattle::shoot(int x, int y, Tank& shootingTank)
{
	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		for (int j = 0; j < allTanks[i].size(); j++)
		{
			if (allTanks[i][j].isComputer())
			{
				allTanks[i][j].simulateShootBy(shootingTank);
			}
		}
	}
	if (board[y][x] == 'O')
	{
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			for (vector<Tank>::iterator iter = allTanks[i].begin(); iter != allTanks[i].end(); iter++)
			{
				const Point& body = iter->getBody();
				if ((body.returnXcord() == x) && (body.returnYcord() == y))
				{
					iter->removeFromBoard();
					allTanks[i].erase(iter);
					activeTankIndexes[i] = 0;
					return;
				}
			}
		}
	}
}

void theTankBattle::setCharAt(int x, int y, char c)
{
	board[y][x] = c;
	IO_Utils::gotoxy(x, y); // Draw a space to clear the position
	if (!silent) {
		cout << c;
	}
}

void theTankBattle::clearCharAt(int x, int y)
{
	setCharAt(x, y, ' ');
}

char theTankBattle::getCharAt(int x, int y) const
{
	return board[y][x];
}

void theTankBattle::showSuspendMessage() const
{
	if (!silent) {
		IO_Utils::gotoxy(10, GameConfig::GAME_HEIGHT / 2);
		cout << "Game paused, press ESC again to continue";
		IO_Utils::gotoxy(10, GameConfig::GAME_HEIGHT / 2 + 1);
		cout << "or X to go back to the main menu";
	}
}

void theTankBattle::showScore() const
{
	if (!silent) {
		std::stringstream ss;
		ss << "Score - ";
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			// the score for each player is the number of tanks that he has
			ss << "Player " << (i + 1) << ": " << allTanks[i].size() << " ";
		}
		string score;
		getline(ss, score);
		IO_Utils::gotoxy(scorePlace.returnXcord(), scorePlace.returnYcord());
		cout << score;
	}
}

bool theTankBattle::hasFirstKBHit(int step) const
{
	if (gameMode != LOAD)
	{
		return _kbhit();
	}
	return (step <= kbHits.size()) && (kbHits[step - 1].size() > 0);
}

char theTankBattle::getFirstKBHit(int step) const
{
	if (gameMode != LOAD)
	{
		return _getch();
	}
	return kbHits[step - 1][0];
}

bool theTankBattle::hasSecondKBHit(int step) const
{
	if (gameMode != LOAD)
	{
		return _kbhit();
	}
	return (step <= kbHits.size()) && (kbHits[step - 1].size() > 1);
}

char theTankBattle::getSecondKBHit(int step) const
{
	if (gameMode != LOAD)
	{
		return _getch();
	}
	return kbHits[step - 1][1];
}

void theTankBattle::run()
{
	bool esc_pressed = false;

	char key = 0;
	char key2 = 0;
	int step = 0;
	GameConfig::eKeys dir;
	do
	{
		step++;
		vector<char> keys;
		showScore();
		if (hasFirstKBHit(step))
		{
			key = getFirstKBHit(step);
			if (gameMode == SAVE) {
				keys.push_back(key);
			}
			if (key == char(GameConfig::eKeys::ESC))
			{
				if (!esc_pressed)
				{
					showSuspendMessage();
				}
				else
				{
					drawBoard();
				}
				esc_pressed = !esc_pressed;
			}
			else if (esc_pressed)
			{
				if ((key == 'X') || (key == 'x'))
				{
					esc_pressed = false;
					break;
				}
				continue;
			}
			else if ((key == char(GameConfig::eKeys::Z)) || (key == char(GameConfig::eKeys::z)))
			{
				// go to the next tank of player 1
				if (allTanks[0].size() > 0)
				{
					activeTankIndexes[0] = (activeTankIndexes[0] + 1) % allTanks[0].size();
				}
				else
				{
					activeTankIndexes[0] = 0;
				}
				continue;
			}
			else if ((key == char(GameConfig::eKeys::M)) || (key == char(GameConfig::eKeys::m)))
			{
				// go to the next tank of player 2
				if (allTanks[1].size() > 0)
				{
					activeTankIndexes[1] = (activeTankIndexes[1] + 1) % allTanks[1].size();
				}
				else
				{
					activeTankIndexes[1] = 0;
				}
				continue;
			}

			if (hasSecondKBHit(step))
			{
				key2 = getSecondKBHit(step);
				if (gameMode == SAVE) {
					keys.push_back(key);
				}
				if ((dir = (GameConfig::eKeys)allTanks[0][activeTankIndexes[0]].getDirection(key,key2)) != GameConfig::eKeys::INVALID)
					allTanks[0][activeTankIndexes[0]].setDirection(dir);
				if ((dir = (GameConfig::eKeys)allTanks[1][activeTankIndexes[1]].getDirection(key , key2)) != GameConfig::eKeys::INVALID)
					allTanks[1][activeTankIndexes[1]].setDirection(dir);

			}
			else 
			{
				if ((dir = (GameConfig::eKeys)allTanks[0][activeTankIndexes[0]].getDirection(key, key2)) != GameConfig::eKeys::INVALID)
					allTanks[0][activeTankIndexes[0]].setDirection(dir);
				if ((dir = (GameConfig::eKeys)allTanks[1][activeTankIndexes[1]].getDirection(key , key2)) != GameConfig::eKeys::INVALID)
					allTanks[1][activeTankIndexes[1]].setDirection(dir);
			}
		}
		else
		{
			if (esc_pressed)
			{
				continue;
			}
		}
		if (gameMode == SAVE) {
			stepsOutputFile << step << ' ' << keys.size() << ' ';
			for (int i = 0; i < keys.size(); i++) {
				stepsOutputFile << keys[i] << ' ';
			}
			stepsOutputFile << endl;
		}

		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			for (int j = 0; j < allTanks[i].size(); j++)
			{
				if (allTanks[i][j].returnDir() == GameConfig::eKeys::W)
				{
					allTanks[i][j].move();
					allTanks[i][j].setDirection(GameConfig::eKeys::S);
					allTanks[i][j].setShot(true);
				}
			}
		}

		key = 0;
		key2 = 0;
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			for (int j = 0; j < allTanks[i].size(); j++)
			{
				if (!allTanks[i][j].didShoot())
				{
					allTanks[i][j].move();
				}
				allTanks[i][j].setShot(false);
				if (!silent) {
					Sleep(100);
				}
			}
		}
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			for (int j = 0; j < allTanks[i].size(); j++)
			{
				if (allTanks[i][j].isComputer())
				{
					allTanks[i][j].simulate(allTanks);
				}
			}
		}
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			for (int j = 0; j < allTanks[i].size(); j++)
			{
				allTanks[i][j].cleanWasShooting();
			}
		}
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			for (vector<Tank>::iterator iter = allTanks[i].begin(); iter != allTanks[i].end(); iter++)
			{
				if (iter->hasMined())
				{
					allTanks[i].erase(iter);
					activeTankIndexes[i] = 0;
					break;
				}
			}
		}
		// if 0 won
		if (allTanks[1].size() == 0)
		{
			if (currentFileIndex < fileNames.size())
			{
				loadNextBoard();
				continue;
			}
			IO_Utils::clear_screen();

			for (int i = 0; i < 6; i++) // Flashing message 3 times
			{
				if (!silent) {
					IO_Utils::setTextColor(i % 2 == 0 ? Color::LIGHTBLUE : Color::WHITE);
					IO_Utils::gotoxy(20, 10);
					cout << "  YOU WON!  ";
					Sleep(300);
				}
			}

			// Fireworks effect (simple random colored stars)
			for (int i = 0; i < 30; i++)
			{
				int x = rand() % (GameConfig::GAME_WIDTH - 2) + 1;
				int y = rand() % (GameConfig::GAME_HEIGHT - 2) + 1;
				if (!silent) {
					IO_Utils::setTextColor(Color(rand() % 15 + 1));
					IO_Utils::gotoxy(x, y);
					cout << "*";
					Sleep(50);
				}
			}
			if (!silent) {
				IO_Utils::setTextColor(Color::LIGHTBLUE);
				IO_Utils::gotoxy(18, 12);
				cout << "CONGRATS, TANK COMMANDER!";

				IO_Utils::setTextColor(Color::WHITE);
				IO_Utils::gotoxy(0, GameConfig::GAME_HEIGHT + 3);
				cout << "Press any key to exit...";
			}
			_getch();

			break;
		}

		// if 1 won
		if (allTanks[0].size() == 0)
		{
			if (currentFileIndex < fileNames.size())
			{
				loadNextBoard();
				continue;
			}
			IO_Utils::clear_screen();

			for (int i = 0; i < 6; i++) // Flashing message 3 times
			{
				if (!silent) {
					IO_Utils::setTextColor(i % 2 == 0 ? Color::LIGHTGREEN : Color::WHITE);
					IO_Utils::gotoxy(20, 10);
					cout << "  YOU WON!  ";
					Sleep(300);
				}
			}

			// Fireworks effect (simple random colored stars)
			if (!silent) {
				for (int i = 0; i < 30; i++)
				{
					int x = rand() % (GameConfig::GAME_WIDTH - 2) + 1;
					int y = rand() % (GameConfig::GAME_HEIGHT - 2) + 1;
					IO_Utils::setTextColor(Color(rand() % 15 + 1));
					IO_Utils::gotoxy(x, y);
					cout << "*";
					Sleep(50);
				}

				IO_Utils::setTextColor(Color::LIGHTGREEN);
				IO_Utils::gotoxy(18, 12);
				cout << "CONGRATS, TANK COMMANDER!";

				IO_Utils::setTextColor(Color::WHITE);
				IO_Utils::gotoxy(0, GameConfig::GAME_HEIGHT + 3);
				cout << "Press any key to exit...";
				_getch();
			}

			break;
		}

	} while (key != ESC);
}

void theTankBattle::drawBoard() const
{
	if (!silent) {
		IO_Utils::clear_screen();
		IO_Utils::gotoxy(0, 0);
		for (int row = 0; row <= GameConfig::GAME_HEIGHT + 1; row++)
		{
			cout << board[row] << endl;
		}
	}
}

void theTankBattle::fillBoard()
{
	// remove all tanks
	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		allTanks[i].clear();
	}
	for (int row = 0; row <= GameConfig::GAME_HEIGHT + 1; row++)
	{
		for (int col = 0; col <= GameConfig::GAME_WIDTH + 1; col++)
		{
			board[row][col] = ' ';
		}
		board[row][GameConfig::GAME_WIDTH + 2] = '\0';
	}

	ifstream input(fileNames[currentFileIndex]);
	if (gameMode == SAVE) {
		std::size_t found = fileNames[currentFileIndex].find_last_of(".");
		string stepsFileName = fileNames[currentFileIndex].substr(0, found) + string(".steps");
		if (stepsOutputFile.is_open()) {
			stepsOutputFile.close();
		}
		stepsOutputFile.open(stepsFileName);
	}
	else if (gameMode == LOAD) {
		std::size_t found = fileNames[currentFileIndex].find_last_of(".");
		string stepsFileName = fileNames[currentFileIndex].substr(0, found) + string(".steps");
		if (stepsInputFile.is_open()) {
			stepsInputFile.close();
		}
		stepsInputFile.open(stepsFileName);
		kbHits.clear();
		// read the steps file
		int step, size;
		char temp;
		while (stepsInputFile) {
			vector<char> hits;
			stepsInputFile >> step >> size;
			for (int i = 0; i < size; i++) {
				stepsInputFile >> temp;
				hits.push_back(temp);
			}
			kbHits.push_back(hits);
		}
	}
	std::string line;
	int lineNumber = 0;
	Tank tank;
	tank.init(this);
	while (std::getline(input, line))
	{
		for (int i = 0; i < line.size(); i++)
		{
			switch (line[i])
			{
			case '1':
				// tank owned by player 1
				board[lineNumber][i] = 'O';
				tank.setPlacement(i, lineNumber);
				if (mode != COMPUTER_VS_COMPUTER)
				{
					tank.setArrowKeys("edqasw");
				}
				else
				{
					tank.setArrowKeys("");
				}
				tank.setColor(Color::LIGHTBLUE);
				tank.setSilent(silent);
				allTanks[0].push_back(tank);
				break;
			case '2':
				// tank owned by player 2
				board[lineNumber][i] = 'O';
				tank.setPlacement(i, lineNumber);
				if (mode == HUMAN_VS_HUMAN)
				{
					tank.setArrowKeys("olujki");
				}
				else
				{
					tank.setArrowKeys("");
				}
				tank.setColor(Color::LIGHTGREEN);
				tank.setSilent(silent);
				allTanks[1].push_back(tank);
				break;
			case '@':
				// mine
				board[lineNumber][i] = '@';
				break;
			case 'L':
				// information line
				board[lineNumber][i] = ' ';
				scorePlace.init(lineNumber, i);
				break;
			default:
				board[lineNumber][i] = line[i];
				break;
			}
		}
		lineNumber++;
	}

	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		for (int j = 0; j < allTanks[i].size(); j++)
		{
			allTanks[i][j].setRandomDirection();
		}
	}
	currentFileIndex++;
}

void theTankBattle::setGameMode(GameMode gameMode) {
	this->gameMode = gameMode;
}

void theTankBattle::setSilent(bool silent) {
	this->silent = silent;
}

void theTankBattle::setCursorPosition(int x, int y)  const {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void theTankBattle::displayMenu() const {
	if (!silent) {
		setCursorPosition(0, 0);
		cout << "================ MAIN MENU ================" << endl;
		cout << "1. Start Game" << endl;
		cout << "8. Instructions and Keys" << endl;
		cout << "9. Exit" << endl;
		cout << "===========================================" << endl;
	}
}

// Function to simulate starting the game

// Function to display instructions and keys
void theTankBattle::displayInstructions() const {
	cout << "================ INSTRUCTIONS =============" << endl;
	cout << "Use the following keys to play the game:" << endl;
	cout << "Arrow keys: Move Tank 1" << endl;
	cout << "'e/E': Right track forward" << endl;
	cout << "'q/Q': Left track forward" << endl;
	cout << "'d/D': Right track backwards" << endl;
	cout << "'a/A': Left track backwards" << endl;
	cout << "'s/S': Stand still" << endl;
	cout << "'w/W': Shoot" << endl;

	cout << "Arrow keys: Move Tank 2" << endl;
	cout << "'o/O': Right track forward" << endl;
	cout << "'u/U': Left track forward" << endl;
	cout << "'l/L': Right track backwards" << endl;
	cout << "'j/J': Left track backwards" << endl;
	cout << "'k/K': Stand still" << endl;
	cout << "'i/I': Shoot" << endl;
	cout << "'ESC': Quit the game" << endl;
	cout << "===========================================" << endl;
	cout << "Press any key to return to the menu." << endl;
	_getch(); // Wait for user to press any key
	IO_Utils::clear_screen();

}

// Function to handle exiting the game
void theTankBattle::exitGame() const {
	IO_Utils::clear_screen();
	cout << "Exiting the game. Goodbye!" << endl;
}
