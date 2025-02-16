#include "Board.h"
#include "GameConstants.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace GameConstants;

enum class GameStates{Painting, Simulation};
GameStates gameState = GameStates::Painting;

void DrawGrid(sf::RenderWindow& window, const std::list<std::pair<int, int>>& activeCells);
void Painting(sf::RenderWindow& window, std::unique_ptr<Board>& board, sf::Clock& inputDelayClock, sf::Clock& clock);
void Simulation(sf::Clock& clock, std::unique_ptr<Board>& board, sf::Clock& inputDelayClock);
void DrawActiveCells(sf::RenderWindow& window, const std::list<std::pair<int, int>>& activeCells);

//Returns the index of the grid at screen position (x, y)
std::optional<std::pair<int, int>> GetGridIndex(int x, int y);

//Returns the screen position of the top left of a row / column on the grid
std::pair<double, double> GetScreenPosition(int row, int col);

void Painting(sf::RenderWindow& window, std::unique_ptr<Board>& board, sf::Clock& inputDelayClock, sf::Clock& clock);
void Simulation(sf::Clock& clock, std::unique_ptr<Board>& board, sf::Clock& inputDelayClock);

int main()
{
    sf::RenderWindow window(sf::VideoMode({ scrWidth, scrHeight }), "Conway's Game Of Life", 
        sf::Style::Titlebar, sf::State::Windowed);

    std::unique_ptr<Board> board = std::make_unique<Board>();
    sf::Clock clock;
    sf::Clock inputDelayClock;
    inputDelayClock.start();

    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        //Check for events
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                std::cout << "Close window event";
                window.close();
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            window.close();

        window.clear(sf::Color::Black);

        DrawGrid(window, board->GetActiveCells());

        if (gameState == GameStates::Painting)
            Painting(window, board, inputDelayClock, clock);
        else if (gameState == GameStates::Simulation)
            Simulation(clock, board, inputDelayClock);

        window.display();
    }
}

void Simulation(sf::Clock& clock, std::unique_ptr<Board>& board, sf::Clock& inputDelayClock)
{
    if (clock.getElapsedTime().asSeconds() >= tickDuration)
    {
        board->UpdateBoard();
        clock.restart();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)
        && inputDelayClock.getElapsedTime().asSeconds() >= inputDelay)
    {
        board->Reset();
        gameState = GameStates::Painting;
        inputDelayClock.restart();
    }
}

void Painting(sf::RenderWindow& window, std::unique_ptr<Board>& board, sf::Clock& inputDelayClock, sf::Clock& clock)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        std::optional cell = GetGridIndex(mousePos.x, mousePos.y);

        if (cell)
            board->AddActiveCell(*cell);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)
        && inputDelayClock.getElapsedTime().asSeconds() >= inputDelay)
    {
        clock.restart();
        gameState = GameStates::Simulation;
        board->StartSimulation();
        inputDelayClock.restart();
    }
}

void DrawGrid(sf::RenderWindow& window, const std::list<std::pair<int, int>>& activeCells)
{
    for (int i = 1; i <= numRows - 1; i++)
    {
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);

        line[0].position = sf::Vector2f(0, i * gridHeight);
        line[1].position = sf::Vector2f(scrWidth, i * gridHeight);

        line[0].color = line[1].color = sf::Color::White;

        window.draw(line);
    }

    for (int i = 1; i <= numCols - 1; i++)
    {
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);

        line[0].position = sf::Vector2f(i * gridWidth, 0);
        line[1].position = sf::Vector2f(i * gridWidth, scrHeight);

        line[0].color = line[1].color = sf::Color::White;

        window.draw(line);
    }

    DrawActiveCells(window, activeCells);
}

void DrawActiveCells(sf::RenderWindow& window, const std::list<std::pair<int, int>>& activeCells)
{
    for (auto cell : activeCells)
    {
        std::pair<double, double> pos = GetScreenPosition(cell.first, cell.second);

        sf::RectangleShape rectangle({ gridWidth, gridHeight });
        rectangle.setPosition(sf::Vector2f(pos.first, pos.second));
        rectangle.setFillColor(sf::Color::Yellow);

        window.draw(rectangle);
    }
}

std::optional<std::pair<int, int>> GetGridIndex(int x, int y)
{
    if (x < 0 || x > GameConstants::scrWidth || y < 0 || y > GameConstants::scrHeight)
        return std::nullopt;

    int row = static_cast<int>(y / GameConstants::gridHeight);
    int col = static_cast<int>(x / GameConstants::gridWidth);

    return std::make_optional<std::pair<int, int>>( row, col );
}

std::pair<double, double> GetScreenPosition(int row, int col)
{
    if (row < 0 || row > numRows || col < 0 || col > numCols)
        throw std::invalid_argument("row or column out of range");

    double x = ((double)col * gridWidth);
    double y = ((double)row * gridHeight);

    return { x, y };
}

void Painting()
{

}