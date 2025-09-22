#include "MiniMax.h"
#include "Evulation.h"
#include <exception>
#include <climits>
#include <iostream> // Для отладки
#define SIZE 8
#define WHITE 1
#define BLACK 2

std::vector<TreeState*> maxValues = {};
std::vector<TreeState*> minValues = {};

int minValue = -999999;
int maxValue = -999999;

// Функция альфа-бета отсечения с отладкой
void alphaBeta(TreeState* node, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || node->nodes.empty()) {
        return;
    }

    if (maximizingPlayer) { // Ход чёрных (максимизация)
        int value = INT_MIN;
        TreeState* bestChild = nullptr;
        for (TreeState* child : node->nodes) {
            alphaBeta(child, depth - 1, alpha, beta, false);
            if (!child->pruned) {
                int childValue = EvulationFunc(child->rootState);
                if (childValue > value) {
                    value = childValue;
                    bestChild = child;
                }
                alpha = std::max(alpha, value);
                if (beta <= alpha) {
                    // Отсекаем оставшиеся узлы
                    for (size_t i = node->nodes.size(); i > 0 && node->nodes[i - 1] != child; i--) {
                        node->nodes[i - 1]->pruned = true;
                    }
                    break;
                }
            }
        }
        // Если все узлы отсечены, оставляем хотя бы один лучший узел
        if (bestChild == nullptr && !node->nodes.empty()) {
            for (TreeState* child : node->nodes) {
                child->pruned = false; // Сбрасываем отсечение для всех узлов
            }
        }
    }
    else { // Ход белых (минимизация)
        int value = INT_MAX;
        TreeState* bestChild = nullptr;
        for (TreeState* child : node->nodes) {
            alphaBeta(child, depth - 1, alpha, beta, true);
            if (!child->pruned) {
                int childValue = EvulationFunc(child->rootState);
                if (childValue < value) {
                    value = childValue;
                    bestChild = child;
                }
                beta = std::min(beta, value);
                if (beta <= alpha) {
                    for (size_t i = node->nodes.size(); i > 0 && node->nodes[i - 1] != child; i--) {
                        node->nodes[i - 1]->pruned = true;
                    }
                    break;
                }
            }
        }
        if (bestChild == nullptr && !node->nodes.empty()) {
            for (TreeState* child : node->nodes) {
                child->pruned = false;
            }
        }
    }
}

void FindMaxValues(TreeState* root, int depth) {
    if (depth != 1) {
        for (int i = 0; i < root->nodes.size(); i++) {
            if (!root->nodes[i]->pruned) {
                FindMaxValues(root->nodes[i], depth - 1);
            }
        }
    }
    else {
        for (int i = 0; i < root->nodes.size(); i++) {
            if (!root->nodes[i]->pruned) {
                int eval = EvulationFunc(root->nodes[i]->rootState);
                if (eval > maxValue) {
                    maxValues.clear();
                    maxValue = eval;
                    maxValues.push_back(root->nodes[i]);
                }
                else if (eval == maxValue) {
                    maxValues.push_back(root->nodes[i]);
                }
            }
        }
    }
}

TreeState* MiniMax(TreeState* root, int depth) {
    if (!root) {
        throw std::runtime_error("Root is null in MiniMax");
    }

    // Сначала применяем альфа-бета отсечение
    alphaBeta(root, depth, INT_MIN, INT_MAX, true);

    // Затем вызываем текущую логику MiniMax
    maxValue = -999999;
    maxValues.clear();
    FindMaxValues(root, depth);

    int currentDepth = depth;
    while (currentDepth > 1) {
        if (currentDepth % 2 == 1) { // Минимизация (ход белых)
            minValues.clear();
            minValue = -999999;
            for (int i = 0; i < maxValues.size(); i++) {
                if (maxValues[i]->prev && !maxValues[i]->prev->pruned) {
                    int eval = EvulationFunc(maxValues[i]->prev->rootState);
                    if (minValue < eval) {
                        minValues.clear();
                        minValue = eval;
                        minValues.push_back(maxValues[i]->prev);
                    }
                    else if (minValue == eval) {
                        minValues.push_back(maxValues[i]->prev);
                    }
                }
            }
        }
        else { // Максимизация (ход чёрных)
            maxValues.clear();
            maxValue = -999999;
            for (int i = 0; i < minValues.size(); i++) {
                if (minValues[i]->prev && !minValues[i]->prev->pruned) {
                    int eval = EvulationFunc(minValues[i]->prev->rootState);
                    if (maxValue < eval) {
                        maxValues.clear();
                        maxValue = eval;
                        maxValues.push_back(minValues[i]->prev);
                    }
                    else if (maxValue == eval) {
                        maxValues.push_back(minValues[i]->prev);
                    }
                }
            }
        }
        currentDepth--;
    }

    if (maxValues.empty()) {
        // Вместо исключения выберем случайный узел из дочерних
        std::cout << "maxValues пуст, выбираем случайный узел" << std::endl;
        for (TreeState* child : root->nodes) {
            child->pruned = false; // Сбрасываем отсечение
            maxValues.push_back(child);
        }
        if (!maxValues.empty()) {
            return maxValues[0];
        }
        else {
            throw std::runtime_error("No valid moves found even after resetting pruning");
        }
    }
    return maxValues[0];
}

int CountBlackCheckers(GameState currState) {
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (currState.board[i][j] == BLACK)
                count++;
        }
    }
    return count;
}

int CountWhiteCheckers(GameState currState) {
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (currState.board[i][j] == WHITE)
                count++;
        }
    }
    return count;
}

TreeState* CreateTree(GameState currState, int depth, TreeState* previous) {
    TreeState* ts = new TreeState;
    ts->rootState = currState;
    ts->prev = previous;

    if (depth == 0) {
        return ts;
    }

    TreeState* nodeState;

    if (depth % 2 == 1) { // Ход чёрных
        if (CountBlackCheckers(currState) < 12) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (currState.board[i][j] == 0) {
                        nodeState = new TreeState;
                        nodeState->rootState = currState;
                        nodeState->rootState.board[i][j] = BLACK;
                        nodeState->prev = ts;
                        ts->nodes.push_back(nodeState);
                    }
                }
            }
        }
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (currState.board[i][j] == BLACK) {
                    for (int x = 0; x < SIZE; x++) {
                        for (int y = 0; y < SIZE; y++) {
                            if (currState.board[x][y] == 0) {
                                nodeState = new TreeState;
                                nodeState->rootState = currState;
                                nodeState->rootState.board[i][j] = 0;
                                nodeState->rootState.board[x][y] = BLACK;
                                nodeState->prev = ts;
                                ts->nodes.push_back(nodeState);
                            }
                        }
                    }
                }
            }
        }
    }
    else { // Ход белых
        if (CountWhiteCheckers(currState) < 12) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (currState.board[i][j] == 0) {
                        nodeState = new TreeState;
                        nodeState->rootState = currState;
                        nodeState->rootState.board[i][j] = WHITE;
                        nodeState->prev = ts;
                        ts->nodes.push_back(nodeState);
                    }
                }
            }
        }
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (currState.board[i][j] == WHITE) {
                    for (int x = 0; x < SIZE; x++) {
                        for (int y = 0; y < SIZE; y++) {
                            if (currState.board[x][y] == 0) {
                                nodeState = new TreeState;
                                nodeState->rootState = currState;
                                nodeState->rootState.board[i][j] = 0;
                                nodeState->rootState.board[x][y] = WHITE;
                                nodeState->prev = ts;
                                ts->nodes.push_back(nodeState);
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < ts->nodes.size(); i++) {
        ts->nodes[i] = CreateTree(ts->nodes[i]->rootState, depth - 1, ts->nodes[i]->prev);
    }

    return ts;
}