#include "tank.h"
#include "Board.h"
#include <vector>

using namespace std;

void Tank::clearCharAt(int x, int y) const
{
	if (board != nullptr)
	{
		board->clearCharAt(x, y);
	}
}

void Tank::init(Board* board)
{
	this->board = board;
	direction = GameConfig::eKeys::S;
	lastDirection = GameConfig::eKeys::R;
	shotMade = false;
	mineStep = false;
}

void Tank::move()
{
	int x = body.returnXcord();
	int y = body.returnYcord();

	if (direction == GameConfig::eKeys::INVALID)
		return;

	// Erase previous cannon head
	if (!silent) {
		if (hasCannon)
		{
			switch (lastDirection)
			{
			case GameConfig::eKeys::U:
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y - 2);
				cout << " ";
				break;
			case GameConfig::eKeys::UR:
				IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 2);
				cout << " ";
				break;
			case GameConfig::eKeys::R:
				IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 1);
				cout << " ";
				break;
			case GameConfig::eKeys::DR:
				IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y);
				cout << " ";
				break;
			case GameConfig::eKeys::D:
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y);
				cout << " ";
				break;
			case GameConfig::eKeys::DL:
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y);
				cout << " ";
				break;
			case GameConfig::eKeys::L:
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 1);
				cout << " ";
				break;
			case GameConfig::eKeys::UL:
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 2);
				cout << " ";
				break;
			default: break;
			}
		}
	}

	// Calculate next position
	int newX = x, newY = y;
	int diff;
	if (hasCannon)
	{
		diff = 2;
	}
	else
	{
		diff = 1;
	}
	switch (direction)
	{
	case GameConfig::eKeys::U:  newY -= diff; break;
	case GameConfig::eKeys::UR: newX += diff; newY -= diff; break;
	case GameConfig::eKeys::R:  newX += diff; break;
	case GameConfig::eKeys::DR: newX += diff; newY += diff; break;
	case GameConfig::eKeys::D:  newY += diff; break;
	case GameConfig::eKeys::DL: newX -= diff; newY += diff; break;
	case GameConfig::eKeys::L:  newX -= diff; break;
	case GameConfig::eKeys::UL: newX -= diff; newY -= diff; break;
	default: break;
	}

	// Check for wall collision
	if (getCharAt(newX, newY) == '#')
	{
		setDirection(GameConfig::eKeys::S); // Stop if wall
		return;
	}
	if (getCharAt(newX, newY) == '@')
	{
		mineStep = true; // Stop if wall

		if (!silent) {
			Sleep(200); // 200 milliseconds
		}
		clearCharAt(newX - 1, newY);
		clearCharAt(newX + 1, newY);
		clearCharAt(newX, newY - 1);
		clearCharAt(newX, newY + 1);
		clearCharAt(newX, newY);
		clearCharAt(body.returnXcord(), body.returnYcord());
		return;
	}

	// Move the tank
	body.draw("  ");
	body.move(direction);
	IO_Utils::setTextColor(color);

	x = body.returnXcord();
	y = body.returnYcord();
	body.draw("O");

	// Draw cannon
	if (hasCannon)
	{
		switch (direction)
		{
		case GameConfig::eKeys::U:
			if (!silent) {
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y - 2);
				cout << "|";
			}
			break;
		case GameConfig::eKeys::UR:
			if (!silent) {
				IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 2);
				cout << "/";
			}
			break;
		case GameConfig::eKeys::R:
			if (!silent) {
				IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 1);
				cout << "-";
			}
			break;
		case GameConfig::eKeys::DR:
			if (!silent) {
				IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y);
				cout << "\\";
			}
			break;
		case GameConfig::eKeys::D:
			if (!silent) {
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y);
				cout << "|";
			}
			break;
		case GameConfig::eKeys::DL:
			if (!silent) {
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y);
				cout << "/";
			}
			break;
		case GameConfig::eKeys::L:
			if (!silent) {
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 1);
				cout << "-";
			}
			break;
		case GameConfig::eKeys::UL:
			if (!silent) {
				IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 2);
				cout << "\\";
			}
			break;
		case GameConfig::eKeys::W:
			// shoot logic here if needed
			shoot();
			break;
		case GameConfig::eKeys::S:
			// Redraw cannon in last direction
			if (!silent) {
				switch (lastDirection)
				{
				case GameConfig::eKeys::U:  IO_Utils::gotoxy(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y - 2); cout << "|"; break;
				case GameConfig::eKeys::UR: IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 2); cout << "/"; break;
				case GameConfig::eKeys::R:  IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 1); cout << "-"; break;
				case GameConfig::eKeys::DR: IO_Utils::gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y);     cout << "\\"; break;
				case GameConfig::eKeys::D:  IO_Utils::gotoxy(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y);     cout << "|"; break;
				case GameConfig::eKeys::DL: IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y);     cout << "/"; break;
				case GameConfig::eKeys::L:  IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 1); cout << "-"; break;
				case GameConfig::eKeys::UL: IO_Utils::gotoxy(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 2); cout << "\\"; break;
				}
			}
			break;
		}
	}

	// Update last direction if not standing still
	if (direction != GameConfig::eKeys::S && direction != GameConfig::eKeys::W)
		lastDirection = direction;

	IO_Utils::setTextColor(Color::WHITE);
	IO_Utils::hideCursor();
}

void Tank::setRandomDirection()
{
	static vector<GameConfig::eKeys> directions;
	static bool directionsInitialized = false;
	if (!directionsInitialized)
	{
		directions.push_back(GameConfig::eKeys::U);
		directions.push_back(GameConfig::eKeys::UR);
		directions.push_back(GameConfig::eKeys::R);
		directions.push_back(GameConfig::eKeys::DR);
		directions.push_back(GameConfig::eKeys::D);
		directions.push_back(GameConfig::eKeys::DL);
		directions.push_back(GameConfig::eKeys::L);
		directions.push_back(GameConfig::eKeys::UL);
	}
	bool foundValidDirection = false;
	for (int i = 0; i < directions.size(); i++)
	{
		if (isValidDirection(directions[i]))
		{
			foundValidDirection = true;
			break;
		}
	}
	if (!foundValidDirection)
	{
		hasCannon = false;
		return;
	}
	GameConfig::eKeys tempDirection = directions[rand() % directions.size()];
	while (!isValidDirection(tempDirection))
	{
		tempDirection = directions[rand() % directions.size()];
	}
	lastDirection = tempDirection;
}

bool Tank::isValidDirection(GameConfig::eKeys direction) const
{
	int x = body.returnXcord();
	int y = body.returnYcord();
	switch (direction)
	{
	case GameConfig::eKeys::U:
		return board->getCharAt(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y - 2) == ' ';
	case GameConfig::eKeys::UR:
		return board->getCharAt(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 2) == ' ';
	case GameConfig::eKeys::R:
		return board->getCharAt(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 1) == ' ';
	case GameConfig::eKeys::DR:
		return board->getCharAt(x + GameConfig::MIN_X, y + GameConfig::MIN_Y) == ' ';
	case GameConfig::eKeys::D:
		return board->getCharAt(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y) == ' ';
	case GameConfig::eKeys::DL:
		return board->getCharAt(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y) == ' ';
		return board->getCharAt(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y) == ' ';
	case GameConfig::eKeys::L:
		return board->getCharAt(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 1) == ' ';
	case GameConfig::eKeys::UL:
		return board->getCharAt(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 2) == ' ';
	}
	// just for the compiler
	return false;
}

bool Tank::threatOnMe(const Tank& tank) const
{
	// return true if the other tak aims on this tank
	GameConfig::eKeys otherLastDirection = tank.getLastDirection();
	int x = body.returnXcord();
	int y = body.returnYcord();
	int otherX = tank.getBody().returnXcord();
	int otherY = tank.getBody().returnYcord();
	switch (otherLastDirection)
	{
	case GameConfig::eKeys::U:
		return (x == otherX) && (y <= otherY);
	case GameConfig::eKeys::UR:
		return (x > otherX) && (y < otherY) && (x - otherX == otherY - y);
	case GameConfig::eKeys::R:
		return (x >= otherX) && (y == otherY);
	case GameConfig::eKeys::DR:
		return (x > otherX) && (y >= otherY) && (x - otherX == y - otherY);
	case GameConfig::eKeys::D:
		return (x == otherX) && (y >= otherY);
	case GameConfig::eKeys::DL:
		return (x < otherX) && (y >= otherY) && (otherX - x == y - otherY);
	case GameConfig::eKeys::L:
		return (x <= otherX) && (y == otherY);
	case GameConfig::eKeys::UL:
		return (x < otherX) && (y < otherY) && (otherX - x == otherY - y);
	default: break;
	}
	return false;
}

void Tank::runAway(const Tank& tank)
{
	GameConfig::eKeys otherLastDirection = tank.getLastDirection();
	int x = body.returnXcord();
	int y = body.returnYcord();
	int otherX = tank.getBody().returnXcord();
	int otherY = tank.getBody().returnYcord();
	switch (otherLastDirection)
	{
	case GameConfig::eKeys::U:
		setDirection(GameConfig::eKeys::R);
		break;
	case GameConfig::eKeys::UR:
		setDirection(GameConfig::eKeys::R);
		break;
	case GameConfig::eKeys::R:
		setDirection(GameConfig::eKeys::U);
		break;
	case GameConfig::eKeys::DR:
		setDirection(GameConfig::eKeys::U);
		break;
	case GameConfig::eKeys::D:
		setDirection(GameConfig::eKeys::R);
		break;
	case GameConfig::eKeys::DL:
		setDirection(GameConfig::eKeys::L);
		break;
	case GameConfig::eKeys::L:
		setDirection(GameConfig::eKeys::D);
		break;
	case GameConfig::eKeys::UL:
		setDirection(GameConfig::eKeys::R);
		break;
	default: break;
	}
}

void Tank::simulateShootBy(Tank& tank)
{
	if (tank.getColor() != color)
	{
		if (threatOnMe(tank))
		{
			// go away
			runAway(tank);
			move();
		}
	}
}

void Tank::simulate(const vector<Tank>(&allTanks)[PLAYERS_NUM])
{
	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		for (int j = 0; j < allTanks[i].size(); j++)
		{
			if (allTanks[i][j].getColor() != color)
			{
				if (allTanks[i][j].threatOnMe(*this))
				{
					// shoot on the enemy
					shoot();
				}
			}
		}
	}
}

bool Tank::isComputer() const
{
	return strlen(arrowKeys) == 0;
}

// using numbers as directions
//{ 0 = U , 1 = UR , 2 = R, 3 = DR , 4 = D , 5 = DL , 6 = L , 7 = UR}
int Tank::getDirection(char key,char key2)
{
	if (isComputer())
	{
		return -1;
	}
	key = std::tolower(key);
	key2 = std::tolower(key2);
	//static int lastDir = 2;
	int lastDir = (int)lastDirection;
	
	if (key == arrowKeys[0] || key2 == arrowKeys[0]) // e 
	{
		if (key2 == arrowKeys[3] || key ==arrowKeys[3]) // e a
		{
			if (direction == GameConfig::eKeys::S)
			{
				return lastDir;
			}
			else if (lastDir != 0 && lastDir!=1)
			{
				lastDir -= 2;
				//lastDirection = (GameConfig::eKeys)lastDir;
				return lastDir;
			}
			else if (lastDir == 0)
			{
				lastDir = 6;
					return lastDir;
			}
			else if (lastDir == 1)
			{
				lastDir = 7;
				return lastDir;
			}
		}
		else if (key2 == arrowKeys[2] || key == arrowKeys[2]) // e q
		{
			if (lastDir == 8)
			{
				lastDir = (int)direction;
				return lastDir;
			}
			else {
				return lastDir;
			}
		}
		else
		{
			if (lastDir == 0)
			{
				lastDir = 7;
				return lastDir;
			}
			else
			{
				lastDir -= 1;
				return lastDir;
			}
		}	
	}
	else if (key == arrowKeys[1] || key2 == arrowKeys[1]) // d
	{
		if (key2 == arrowKeys[3] || key == arrowKeys[3]) // d a
		{
			if (lastDir != 4 && lastDir != 5 && lastDir != 6 && lastDir != 7)
			{
				lastDir += 4;
				return lastDir;
			}
			else if (lastDir == 4)
			{
				lastDir = 0;
				return lastDir;
			}
			else if (lastDir == 5)
			{
				lastDir = 1;
				return lastDir;
			}
			else if (lastDir == 6)
			{
				lastDir = 2;
				return lastDir;
			}
			else if (lastDir == 7)
			{
				lastDir = 3;
				return lastDir;
			}
		}
		else if (key2 == arrowKeys[2] || key == arrowKeys[2]) // d q
		{
			if (lastDir != 6 && lastDir != 7)
			{
				return lastDir += 2;
				return lastDir;
			}
			else if (lastDir == 6)
			{
				lastDir = 1;
				return lastDir;
			}
			else if (lastDir == 7)
			{
				lastDir = 2;
				return lastDir;
			}
		}
		else
		{
			if (lastDir == 7)
			{
				lastDir = 0;
				return lastDir;
			}
			else
			{
				lastDir += 1;
				return lastDir;
			}
		}
	}
	else if (key == arrowKeys[2] || key2 == arrowKeys[2]) // q 
	{
		if (key2 == arrowKeys[0] || key == arrowKeys[0]) // q e 
		{
			return lastDir;
		}
		else if (key2 == arrowKeys[1] || key == arrowKeys[1]) // q d
		{
			if (lastDir != 6 && lastDir != 7)
			{
				return lastDir += 2;
				return lastDir;
			}
			else if (lastDir == 6)
			{
				lastDir = 1;
				return lastDir;
			}
			else if (lastDir == 7)
			{
				lastDir = 2;
				return lastDir;
			}
		}
		else
		{
			if (lastDir == 7)
			{
				lastDir = 0;
				return lastDir;
			}
			else
			{
				lastDir += 1;
				return lastDir;
			}
		}
	}

	else if (key == arrowKeys[3] || key2 == arrowKeys[3]) // a 
	{
		if (key2 == arrowKeys[0] || key == arrowKeys[0]) // a e 
		{
			if (lastDir != 0 && lastDir != 1)
			{
				lastDir -= 2;
				return lastDir;
			}
			else if (lastDir == 0)
			{
				lastDir = 6;
				return lastDir;
			}
			else if (lastDir == 1)
			{
				lastDir = 7;
				return lastDir;
			}
		}
		else if (key2 == arrowKeys[2] || key == arrowKeys[2]) // a d
		{
			if (lastDir != 4 && lastDir != 5 && lastDir != 6 && lastDir != 7)
			{
				lastDir += 4;
				return lastDir;
			}
			else if (lastDir == 4)
			{
				lastDir = 0;
				return lastDir;
			}
			else if (lastDir == 5)
			{
				lastDir = 1;
				return lastDir;
			}
			else if (lastDir == 6)
			{
				lastDir = 2;
				return lastDir;
			}
			else if (lastDir == 7)
			{
				lastDir = 3;
				return lastDir;
			}
		}
		else
		{
			if (lastDir == 0)
			{
				lastDir = 7;
				return lastDir;
			}
			else
			{
				lastDir -= 1;
				return lastDir;
			}
		}
	}
	else if (key == arrowKeys[4] || key2 == arrowKeys[4])
	{
		if (direction != GameConfig::eKeys::S)
		{
			lastDirection = direction;
			direction = GameConfig::eKeys::S;
		}
		return 8;

	}
	else if (key == arrowKeys[5] || key2 == arrowKeys[5])
	{
		if (direction == GameConfig::eKeys::S)
		{
			direction = GameConfig::eKeys::W;
			return 9;
		}

	}
	return -1;
}

void Tank::setPlacement(int x, int y)
{
	body.init(x, y);
}

char Tank::getCharAt(int x, int y) const
{
	if (board != nullptr)
	{
		return board->getCharAt(x, y);
	}
	else
	{
		return ' ';
	}
}

void Tank::removeFromBoard()
{
	int x = body.returnXcord();
	int y = body.returnYcord();
	board->clearCharAt(x, y);
	if (hasCannon)
	{
		switch (lastDirection)
		{
		case GameConfig::eKeys::U:  board->clearCharAt(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y - 2);break;
		case GameConfig::eKeys::UR: board->clearCharAt(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 2);break;
		case GameConfig::eKeys::R:  board->clearCharAt(x + GameConfig::MIN_X, y + GameConfig::MIN_Y - 1);break;
		case GameConfig::eKeys::DR: board->clearCharAt(x + GameConfig::MIN_X, y + GameConfig::MIN_Y);break;
		case GameConfig::eKeys::D:  board->clearCharAt(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y);break;
		case GameConfig::eKeys::DL: board->clearCharAt(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y);break;
		case GameConfig::eKeys::L:  board->clearCharAt(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 1);break;
		case GameConfig::eKeys::UL: board->clearCharAt(x + GameConfig::MIN_X - 2, y + GameConfig::MIN_Y - 2);break;
		default: break;
		}
	}
}

void Tank::shoot()
{
	//if (direction != GameConfig::eKeys::S)
	//	return; // Only shoot when tank is stationary
	if (!hasCannon)
	{
		return;
	}
	int x = body.returnXcord();
	int y = body.returnYcord();
	int dx = 0, dy = 0;

	// Determine direction increments based on lastDirection
	switch (lastDirection)
	{
	case GameConfig::eKeys::U:  dy = -1; break;
	case GameConfig::eKeys::UR: dx = 1; dy = -1; break;
	case GameConfig::eKeys::R:  dx = 1; break;
	case GameConfig::eKeys::DR: dx = 1; dy = 1; break;
	case GameConfig::eKeys::D:  dy = 1; break;
	case GameConfig::eKeys::DL: dx = -1; dy = 1; break;
	case GameConfig::eKeys::L:  dx = -1; break;
	case GameConfig::eKeys::UL: dx = -1; dy = -1; break;
	default: return;
	}

	int shellX = x + GameConfig::MIN_X - 1 + dx;
	int shellY = y + GameConfig::MIN_Y - 1 + dy;

	// Move the shell until it hits a wall or edge
	while (getCharAt(shellX, shellY) != '#' && shellX > 0 && shellY > 0 && shellX < 80 && shellY < 25)
	{
		if (!silent) {
			IO_Utils::gotoxy(shellX, shellY);
			cout << "*";
			Sleep(30);
			IO_Utils::gotoxy(shellX, shellY);
			cout << " ";
		}
		shellX += dx;
		shellY += dy;

		if (getCharAt(shellX, shellY) == 'O' && shellX > 0 && shellY > 0 && shellX < 80 && shellY < 25)
		{
			board->shoot(shellX, shellY, *this);
		}
	}
}

void Tank::setSilent(bool silent) {
	this->silent = silent;
}
