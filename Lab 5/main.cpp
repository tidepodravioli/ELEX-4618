#pragma once
#include "CSnakeGame.hpp"


#define WINDOW_SIZE 800
int main()
{
    CSnakeGame snake = CSnakeGame(cv::Size(1000, 800), 6);
    snake.run();
}