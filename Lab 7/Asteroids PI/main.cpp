#include <headers/CAsteroidGamePi.hpp>

int main()
{
    CAsteroidGamePi game(cv::Size(1000,800));
    game.run();

    return 0;
}