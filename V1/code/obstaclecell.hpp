#pragma once //include guard

#include "cell.hpp"

class ObstacleCell : public Cell{
    
    public :

    ObstacleCell(bool state, int x, int y);

    void setAlive(bool state) override;
    void toggleAlive() override;
    bool computeNextState(int aliveNeighborsCount) const override;

};