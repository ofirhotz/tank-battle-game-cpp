#pragma once
#include "tank.h"
#include "Board.h"

#include <vector>
#include <string>
#include <fstream>

using namespace std;

// game modes
#define HUMAN_VS_HUMAN 1
#define HUMAN_VS_COMPUTER 2
#define COMPUTER_VS_COMPUTER 3

enum GameMode
{
	REGULAR,
	SAVE,
	LOAD
};

class theTankBattle : public Board
{
	Point scorePlace;
	enum { ESC = 27 };
	char board[GameConfig::GAME_HEIGHT + 2][GameConfig::GAME_WIDTH + 3];
	vector<string> fileNames;
	vector<Tank> allTanks[PLAYERS_NUM];
	int activeTankIndexes[PLAYERS_NUM] = { 0 };
	int currentFileIndex;
	int mode;
	GameMode gameMode = REGULAR;
	bool silent = false;
	ofstream stepsOutputFile;
	ifstream stepsInputFile;
	vector<vector<char>> kbHits;
	void setCursorPosition(int x, int y) const;
	void fillBoard();
	void drawBoard() const;
	void displayFileNamesMenu() const;
	void displayPlayersTypesMenu()  const;
	bool loadNextBoard();
	void showScore() const;
	bool hasFirstKBHit(int step) const;
	char getFirstKBHit(int step) const;
	bool hasSecondKBHit(int step) const;
	char getSecondKBHit(int step) const;

public:
	theTankBattle();
	~theTankBattle();
	bool init();
	void run();

	void displayMenu() const;
	void displayInstructions() const;
	void exitGame() const;
	void showSuspendMessage() const;

	virtual void clearCharAt(int x, int y);
	virtual char getCharAt(int x, int y) const;
	virtual void setCharAt(int x, int y, char c);
	virtual void shoot(int x, int y, Tank& shootingTank);

	void setGameMode(GameMode gameMode);
	void setSilent(bool silent);
};

