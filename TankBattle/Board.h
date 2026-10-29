#pragma once

class Tank;

class Board
{
public:
    virtual void clearCharAt(int x, int y) = 0;
    virtual char getCharAt(int x, int y) const = 0;
    virtual void setCharAt(int x, int y, char c) = 0;
    virtual void shoot(int x, int y, Tank& shootingTank) = 0;
};
