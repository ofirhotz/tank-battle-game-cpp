#include "point.h"
#include <iostream>
using namespace std;

void Point::init(int x, int y)
{
	this->x = x;
	this->y = y;

	//diff_x = 1;
	//diff_y = 0;
}
void Point::draw(string s ) const
{
	IO_Utils::gotoxy(x + GameConfig::MIN_X - 1, y + GameConfig::MIN_Y - 1);
	cout << s;

	/*gotoxy(x + GameConfig::MIN_X - 1 + 1, y + GameConfig::MIN_Y - 1);
	
	std::cout << static_cast<char>(cannon);*/

	
}
void Point::move(GameConfig::eKeys key)
{
	static GameConfig::eKeys lastDir = GameConfig::eKeys::INVALID;
	switch (key)
	{
	   case GameConfig::eKeys::U:
		diff_x = 0;
		diff_y = -1;
		break;
	   case GameConfig::eKeys::UR:
		   diff_x = 1;
		   diff_y = -1;
		   break;
	   case GameConfig::eKeys::R:
		   diff_x = 1;
		   diff_y = 0;
		   break;
	   case GameConfig::eKeys::DR:
		   diff_x = 1;
		   diff_y = 1;
		   break;
	   case GameConfig::eKeys::D:
		   diff_x = 0;
		   diff_y = 1;
		   break;
	   case GameConfig::eKeys::DL:
		   diff_x = -1;
		   diff_y = 1;
		   break;
	   case GameConfig::eKeys::L:
		   diff_x = -1;
		   diff_y = 0;
		   break;
	   case GameConfig::eKeys::UL:
		   diff_x = -1;
		   diff_y = -1;
		   break;
	   case GameConfig::eKeys::S:
		   diff_x = 0;
		   diff_y = 0;
		   break;
	   case GameConfig::eKeys::W:
		   diff_x = 0;
		   diff_y = 0;
		   break;
			
	}

	lastDir = key;

	x += diff_x;
	if (x > GameConfig::GAME_WIDTH-1 )
	{
		x = 1;
		
	}
	else if (x == 0)
	{
		
		x = GameConfig::GAME_WIDTH - 1;
	}
	y += diff_y;
	
	if (y > GameConfig::GAME_HEIGHT -1 )
	{
		
		y = 1;
		
		
	}
	else if (y == 0)
	{
		IO_Utils::gotoxy(x, 0);
		cout << "#";
		y = GameConfig::GAME_HEIGHT - 1;
	}
	
	
}

int Point::returnXcord() const
{
	return x;
}
int Point::returnYcord() const
{
	return y;
}
