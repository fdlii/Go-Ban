#pragma once
#define SIZE 8

struct GameState {
    int board[SIZE][SIZE];
    int WhiteScore;
    int BlackScore;
};

int EvulationFunc(GameState&);