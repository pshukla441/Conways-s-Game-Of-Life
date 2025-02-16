#pragma once
#include <utility>
#include <stdexcept>
#include <chrono>

namespace GameConstants
{
    constexpr int numRows = 40;
    constexpr int numCols = 40;

    constexpr double tickDuration = 0.05; //in seconds
    constexpr double inputDelay = 0.5;

    constexpr int scrWidth = 1280;
    constexpr int scrHeight = 720;
    const double gridHeight = static_cast<double>(scrHeight) / numRows;
    const double gridWidth = static_cast<double>(scrWidth) / numCols;
}

