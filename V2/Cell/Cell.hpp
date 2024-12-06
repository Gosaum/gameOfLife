#pragma once //include guard

#include <string>
using namespace std;

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

class StandardCell : public Cell{ 
    
    public :
    
    StandardCell(bool state, int x, int y); 
    
};

class ObstacleCell : public Cell{
    
    public :

    ObstacleCell(bool state, int x, int y);

    void setAlive(bool state) override;
    void toggleAlive() override;
    bool computeNextState(int aliveNeighborsCount) const override;

};

class CellFactory {

    public:

    static Cell* createCell(const string& cellType, int x, int y);
};
