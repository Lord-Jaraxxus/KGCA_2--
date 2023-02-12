#include <iostream> // cout, etc
#include <windows.h> //min, max, etc
#include <conio.h> // getch
#include "k_GameCore.h"

int main()
{ 
    k_GameCore* game = new k_GameCore3D;
    game->Run();

    k_GameCore* game2 = new k_GameCore2D;
    game2->Run();

    std::cout << "Game Ending!\n";

}


