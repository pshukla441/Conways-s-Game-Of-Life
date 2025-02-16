#pragma once

#include <vector>
#include <utility>
#include <list>
#include <array>
#include "GameConstants.h"

enum class GridState { On, Off };

//can maybe implement this as a sparse matrix instead?
//For now, you can only define the game state at the start
class Board
{
public:
    Board();

    void UpdateBoard();

    void DrawBoard() const;

    void AddActiveCell(std::pair<int, int> cell);

    void StartSimulation();

    void Reset();

    std::list<std::pair<int, int>> GetActiveCells() const;

private:
    bool running;

    std::vector<std::vector<GridState>> grid;

    //This only contains the active cells before simulation begins. Afterwards, call GetActiveCells()
    std::list<std::pair<int, int>> activeCells;

    std::array<std::pair<int, int>, 8> directions =
    { { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} } };

    int GetNeighbourCount(int row, int col) const;
};