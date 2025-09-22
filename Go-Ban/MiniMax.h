#pragma once
#include "Checker.h"
#include <vector>
#include "Evulation.h"

struct TreeState {
    GameState rootState;
    std::vector<TreeState*> nodes;
    TreeState* prev;
    bool pruned = false;

    ~TreeState() {
        for (TreeState* node : nodes) {
            delete node;
        }
    }
};

TreeState* CreateTree(GameState, int, TreeState*);
TreeState* MiniMax(TreeState*, int);
void FindMaxValues(TreeState*, int);
int CountBlackCheckers(GameState);
int CountWhiteCheckers(GameState);