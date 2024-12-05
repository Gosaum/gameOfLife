#pragma once //include guard

class Cell{

    protected :

    bool alive;
    int x;
    int y;
    Cell(bool state, int x, int y);

    public :

    bool isAlive() const;
    int getX() const;
    int getY() const;

    virtual void setAlive(bool state);
    virtual void toggleAlive();

    virtual bool computeNextState(int aliveNeighborsCount) const;
};