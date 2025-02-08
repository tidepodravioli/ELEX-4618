#pragma once
#include "CSnakeGame.hpp"


#define WINDOW_SIZE 800
int main()
{
    CSnakeGame snake = CSnakeGame(cv::Size(WINDOW_SIZE, WINDOW_SIZE), 6);
    snake.run();
}