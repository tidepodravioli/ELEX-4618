#include "CSnakeGame.hpp"

#define WINDOW_SIZE 800
int main()
{
    cv::Size size(WINDOW_SIZE, WINDOW_SIZE);
    CSnakeGame snake = CSnakeGame(size, 6);

    snake.run();
}