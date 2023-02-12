#include <iostream> // cout, etc
#include <windows.h> //min, max, etc
#include <conio.h> // getch
#include "k_GameCore.h"

int main()
{
    k_GameCore game;
    game.Run();
    game.Run2D();
    std::cout << "Game Ending!\n";

}


