#include <SFML/Graphics.hpp>
#include "Checker.h"
#include "Evulation.h"
#include "MiniMax.h"
#include <iostream>
#include <vector>
#include <utility>
#define SIZE 8
#define WHITE 1
#define BLACK 2
using namespace sf;
using namespace std;

Texture table;
Texture BlackChecker;
Texture WhiteChecker;

GameState CurrentState;

vector<Checker> figures;                                        // вектор со спрайтами для отрисовки

int countLine(const int [SIZE][SIZE], int, int, int, int, int);

int main() {

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            CurrentState.board[i][j] = 0; // Инициализируем доску нулями
        }
    }
    CurrentState.WhiteScore = 0;
    CurrentState.BlackScore = 0;

    table.loadFromFile("assets/Доска.png");
    BlackChecker.loadFromFile("assets/Чёрная шашка.png");
    WhiteChecker.loadFromFile("assets/Белая шашка.png");

    Sprite Black;
    Sprite White;
    Sprite Table;
    Table.setTexture(table);
    Table.setPosition(476, 176);                                // центрируем доску
    Black.setTexture(BlackChecker);
    White.setTexture(WhiteChecker);


    Checker bc1(Black, 2, 100, 50, 0.3648);
    Checker bc2(Black, 2, 200, 50, 0.3648);
    Checker bc3(Black, 2, 300, 50, 0.3648);
    Checker bc4(Black, 2, 400, 50, 0.3648);
    Checker bc5(Black, 2, 500, 50, 0.3648);
    Checker bc6(Black, 2, 600, 50, 0.3648);
    Checker bc7(Black, 2, 700, 50, 0.3648);
    Checker bc8(Black, 2, 800, 50, 0.3648);
    Checker bc9(Black, 2, 900, 50, 0.3648);
    Checker bc10(Black, 2, 1000, 50, 0.3648);
    Checker bc11(Black, 2, 1100, 50, 0.3648);
    Checker bc12(Black, 2, 1200, 50, 0.3648);
    Checker wc1(White, 1, 100, 900, 0.3648);
    Checker wc2(White, 1, 200, 900, 0.3648);
    Checker wc3(White, 1, 300, 900, 0.3648);
    Checker wc4(White, 1, 400, 900, 0.3648);
    Checker wc5(White, 1, 500, 900, 0.3648);
    Checker wc6(White, 1, 600, 900, 0.3648);
    Checker wc7(White, 1, 700, 900, 0.3648);
    Checker wc8(White, 1, 800, 900, 0.3648);
    Checker wc9(White, 1, 900, 900, 0.3648);
    Checker wc10(White, 1, 1000, 900, 0.3648);
    Checker wc11(White, 1, 1100, 900, 0.3648);
    Checker wc12(White, 1, 1200, 900, 0.3648);
    figures.push_back(bc1);
    figures.push_back(bc2);
    figures.push_back(bc3);
    figures.push_back(bc4);
    figures.push_back(bc5);
    figures.push_back(bc6);
    figures.push_back(bc7);
    figures.push_back(bc8);
    figures.push_back(bc9);
    figures.push_back(bc10);
    figures.push_back(bc11);
    figures.push_back(bc12);
    figures.push_back(wc1);
    figures.push_back(wc2);
    figures.push_back(wc3);
    figures.push_back(wc4);
    figures.push_back(wc5);
    figures.push_back(wc6);
    figures.push_back(wc7);
    figures.push_back(wc8);
    figures.push_back(wc9);
    figures.push_back(wc10);
    figures.push_back(wc11);
    figures.push_back(wc12);

    bool isMoving = 0;
    Checker* selected = nullptr;
    Vector2f PrevCheckerPos(0, 0);

    bool visited[SIZE][SIZE] = { {false} };
    int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };

    RenderWindow window(VideoMode(1600, 1000), "Go-Ban");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {      // если нажата левая кнопка
                Vector2i mousepos = Mouse::getPosition(window);
                Vector2f mousePosF(static_cast<float>(mousepos.x), static_cast<float>(mousepos.y));
                if (isMoving == 0) {                                                                        // если фигура не выбрана, ищем ту на которой произошёл клик
                    for (int i = 0; i < 24; i++) {
                        FloatRect bound = figures[i].sprite.getGlobalBounds();
                        if (bound.contains(mousePosF.x, mousePosF.y) && figures[i].color == 1) {            // только для белых*
                            PrevCheckerPos = figures[i].sprite.getPosition();                               // сохраняем фигуру и её предыдущую позицию
                            selected = &figures[i];
                            isMoving = 1;
                            break;
                        }
                    }
                }
                else {
                    if (selected) {
                        FloatRect tableBound = Table.getGlobalBounds();
                        if (tableBound.contains(selected->sprite.getPosition())) {                                                  // если фигура в пределах доски
                            selected->sprite.setPosition((mousepos.x - 476) / 81 * 81 + 476, (mousepos.y - 176) / 81 * 81 + 176);   // корр-ем позицию чтоб стояла по ценру клетки
                            CurrentState.board[(mousepos.y - 176) / 81][(mousepos.x - 476) / 81] = selected->color;

                            pair<int, int> addedPosition = { -1, -1 };
                            pair<int, int> removedPosition = { -1, -1 };
                            //минимакс
                            TreeState* currState = CreateTree(CurrentState, 1, nullptr);
                            TreeState* bestState = MiniMax(currState, 1);
                            GameState prevState = CurrentState;
                            CurrentState = bestState->rootState;

                            for (int i = 0; i < SIZE; i++) {
                                for (int j = 0; j < SIZE; j++) {
                                    if (prevState.board[i][j] == 2 && CurrentState.board[i][j] == 0)
                                        removedPosition = { i, j };
                                    if (prevState.board[i][j] == 0 && CurrentState.board[i][j] == 2) {
                                        addedPosition = { i, j };
                                    }
                                }
                            }

                            if (removedPosition.first == -1) {
                                FloatRect tableBound = Table.getGlobalBounds();
                                for (int i = 0; i < 12; i++) {
                                    if (!tableBound.contains(figures[i].sprite.getPosition())) {
                                        figures[i].sprite.setPosition(addedPosition.second * 81 + 476, addedPosition.first * 81 + 176);
                                        break;
                                    }
                                }
                            }
                            else {
                                for (int i = 0; i < 12; i++) {
                                    if (figures[i].sprite.getGlobalBounds().contains(removedPosition.second * 81 + 476, removedPosition.first * 81 + 176)) {
                                        figures[i].sprite.setPosition(addedPosition.second * 81 + 476, addedPosition.first * 81 + 176);
                                    }
                                }
                            }

                            delete currState;

                            for (int i = 0; i < SIZE; i++) {
                                for (int j = 0; j < SIZE; j++) {
                                    if (!visited[i][j] && (CurrentState.board[i][j] == WHITE || CurrentState.board[i][j] == BLACK)) {
                                        int color = CurrentState.board[i][j];
                                        for (int d = 0; d < 4; d++) {
                                            int dx = directions[d][0];
                                            int dy = directions[d][1];
                                            int length = countLine(CurrentState.board, i, j, dx, dy, color);
                                            if (length >= 2) {
                                                if (length >= 5) {
                                                    if (color == WHITE)
                                                        CurrentState.WhiteScore += 1;
                                                    else
                                                        CurrentState.BlackScore += 1;
                                                }
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

                            for (int i = 0; i < SIZE; i++) {
                                for (int j = 0; j < SIZE; j++) {
                                    visited[i][j] = false;
                                }
                            }
                        }

                        selected = nullptr;     // обнуляем параметры для завершения хода
                        isMoving = 0;

                        if (CurrentState.BlackScore >= 10) {
                            cout << "Black wins!" << endl;
                            exit(1);
                        }
                        if (CurrentState.WhiteScore >= 10) {
                            cout << "White wins!" << endl;
                            exit(1);
                        }

                        cout << "Statistics:" << endl;
                        cout << "Evuluation: " << EvulationFunc(CurrentState) << endl;
                        cout << "BlackScore: " << CurrentState.BlackScore << endl;
                        cout << "WhiteScore: " << CurrentState.WhiteScore << endl;
                    }
                }
            }
        }

        if (selected && isMoving) {                                 // если фигура выбрана, она следует за курсором
            Vector2i mousepos = Mouse::getPosition(window);
            Vector2f mousePosF(static_cast<float>(mousepos.x), static_cast<float>(mousepos.y));
            selected->sprite.setPosition(mousePosF);
        }

        window.clear(Color::Green);

        window.draw(Table);
        for (const auto& checker : figures) {
            window.draw(checker.sprite);
        }

        window.display();
    }
    return 0;
}