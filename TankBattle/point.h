#pragma once
#include <string>
#include "io_utils.h"
#include "gameConfig.h"


using namespace std;

class Point
{
	int x;
	int y;
	int diff_x;
	int diff_y;

public :
	
	void init(int x, int y);
	void draw(string s) const;
	void move(GameConfig::eKeys key);
	int returnXcord() const;
	int returnYcord() const;
};

