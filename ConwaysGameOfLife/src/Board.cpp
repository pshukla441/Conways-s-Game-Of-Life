#include "Board.h"
#include <iostream>
#include <stdexcept>
#include <list>

Board::Board() :
    grid(GameConstants::numRows, std::vector<GridState>(GameConstants::numCols, GridState::Off)),
    directions({ { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} } }),
    running(false)
{};

void Board::UpdateBoard()
{
    if (!running)
    {
        std::cerr << "Can't update the board before starting it running\n";
        return;
    }

    //For active cells:
        //Each cell with 1 or no neighbours dies
        //Each cell with 4 or more neighbours dies
        //Each cell with 2 or three neighbours survives

    //For inactive cells:
        //Each cell with 3 neighbours becomes populated

    std::list<std::pair<int, int>> toDelete;
    std::list<std::pair<int, int>> toActivate;

    for (int r = 0; r < grid.size(); r++)
    {
        for (int c = 0; c < grid[0].size(); c++)
        {
            int neighbourCount = GetNeighbourCount(r, c);

            if (grid[r][c] == GridState::On)
            {
                if (neighbourCount <= 1)
                {
                    toDelete.push_back({ r, c });
                }
                else if (neighbourCount >= 4)
                {
                    toDelete.push_back({ r, c });
                }
            }
            else if (grid[r][c] == GridState::Off && neighbourCount == 3)
            {
                toActivate.push_back({ r, c });
            }
        }
    }

    for (std::pair<int, int> cell : toDelete)
    {
        grid[cell.first][cell.second] = GridState::Off;
    }

    for (std::pair<int, int> cell : toActivate)
    {
        grid[cell.first][cell.second] = GridState::On;
    }
}

int Board::GetNeighbourCount(int row, int col) const
{
    int neighbourCount = 0;

    for (int i = 0; i < directions.size(); i++)
    {
        int r = row + directions[i].first;
        int c = col + directions[i].second;

        if (r < 0 || c < 0 || r >= grid.size() || c >= grid[0].size())
            continue;

        if (grid[r][c] == GridState::On)
            neighbourCount++;
    }

    return neighbourCount;
}

void Board::DrawBoard() const
{
    for (int r = 0; r < grid.size(); r++)
    {
        for (int c = 0; c < grid[0].size(); c++)
        {
            if (grid[r][c] == GridState::On)
                std::cout << 1 << " ";
            else
                std::cout << 0 << " ";
        }

        std::cout << "\n";
    }
}

std::list<std::pair<int, int>> Board::GetActiveCells() const
{
    if (!running)
        return activeCells;

    std::list<std::pair<int, int>> activeCells;

    for (int r = 0; r < grid.size(); r++)
    {
        for (int c = 0; c < grid[0].size(); c++)
        {
            if (grid[r][c] == GridState::On)
                activeCells.emplace_back(std::pair( r, c ));
        }
    }

    return activeCells;
}

void Board::AddActiveCell(std::pair<int, int> cell)
{
    if (std::find(activeCells.begin(), activeCells.end(), cell) == activeCells.end())
        activeCells.emplace_back(cell);
}

void Board::StartSimulation() 
{
    for (auto cell : activeCells)
    {
        grid[cell.first][cell.second] = GridState::On;
    }

    running = true;
}

void Board::Reset()
{
    activeCells = std::list<std::pair<int, int>>();
    running = false;
    grid = std::vector<std::vector<GridState>>
        (GameConstants::numRows, std::vector<GridState>(GameConstants::numCols, GridState::Off));
}