#pragma once
#include "point.h"
#include "io_utils.h"
#include <cctype>

#include <vector>

using namespace std;
class Board;

#define DIRECTIONS_NUMBER 6

class Tank
{
	Point body; 
	GameConfig::eKeys direction = GameConfig::eKeys::S;
    GameConfig::eKeys lastDirection = GameConfig::eKeys::R;
	char arrowKeys[DIRECTIONS_NUMBER];
    bool shotMade = false;
    bool wasShooting = false;
    bool mineStep = false;
    bool hasCannon = true;
	Color color;
    Board* board = nullptr;
    bool silent = false;

    void clearCharAt(int x, int y) const;
    char getCharAt(int x, int y) const;
    bool isValidDirection(GameConfig::eKeys direction) const;
public:
    void init(Board* board);
	void setArrowKeys(const char* keys)
    {
        if (strlen(keys) == DIRECTIONS_NUMBER)
        {
            arrowKeys[0] = keys[0];  // RIGHT TRACK FORWARD
            arrowKeys[1] = keys[1];  // RIGHT TRACK BACKWARDS
            arrowKeys[2] = keys[2];  // LEFT TRACK FORWARD
            arrowKeys[3] = keys[3];  // LEFT TRACK BACKWARDS
            arrowKeys[4] = keys[4];  // STAY
            arrowKeys[5] = keys[5];  // SHOOT
        }
        else
        {
            arrowKeys[0] = '\0';
        }
	}
    void removeFromBoard();
    const Point& getBody() const
    {
        return body;
    }

    void setColor(Color c)
    {
		color = c;
	}
    Color getColor() const
    {
        return color;
    }
	void move();

	int getDirection(char key , char key2);

    void shoot();

    bool hasMined() const
    {
        return mineStep;
    }

    bool didShoot()  const
    {
        return shotMade;
    }

    void setShot(bool bl)
    {
        shotMade = bl;
    }
    void cleanWasShooting()
    {
        wasShooting = false;
    }
    bool isWasShooting() const
    {
        return wasShooting;
    }

	void setDirection(GameConfig::eKeys dir)
    {
		direction = dir;
	}

    GameConfig::eKeys returnDir() const
    {
        return direction;
    }

	void setPlacement(int x, int y);
    void setRandomDirection();
    bool isComputer() const;
    void simulate(const vector<Tank> (&allTanks)[PLAYERS_NUM]);
    GameConfig::eKeys getLastDirection() const
    {
        return lastDirection;
    }
    bool threatOnMe(const Tank&) const;
    void runAway(const Tank&);
    void simulateShootBy(Tank& tank);
    void setSilent(bool silent);
};

