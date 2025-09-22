#include "Evulation.h"
#include <iostream>
const int WHITE = 1;
const int BLACK = 2;

int countLine(const int board[SIZE][SIZE], int x, int y, int dx, int dy, int color) {
    int count = 0;
    int i = x, j = y;
    while (i >= 0 && i < SIZE && j >= 0 && j < SIZE && board[i][j] == color) {
        count++;
        i += dx;
        j += dy;
    }
    return count;
}

int EvulationFunc(GameState& state) {
    int score = 0;

    // Подсчёт линий (оставляем как есть)
    bool visited[SIZE][SIZE] = { {false} };
    int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (!visited[i][j] && (state.board[i][j] == WHITE || state.board[i][j] == BLACK)) {
                int color = state.board[i][j];
                int multiplier = (color == BLACK) ? 1 : -1;
                for (int d = 0; d < 4; d++) {
                    int dx = directions[d][0];
                    int dy = directions[d][1];
                    int length = countLine(state.board, i, j, dx, dy, color);
                    if (length >= 2) {
                        if (length == 2) score += multiplier * 5;
                        else if (length == 3) score += multiplier * 20;
                        else if (length == 4) score += multiplier * 50;
                        int ni = i, nj = j;
                        for (int k = 0; k < length; k++) {
                            if (ni >= 0 && ni < SIZE && nj >= 0 && nj < SIZE) {
                                visited[ni][nj] = true;
                            }
                            ni += dx;
                            nj += dy;
                        }
                    }
                }
            }
        }
    }

    return score;
}