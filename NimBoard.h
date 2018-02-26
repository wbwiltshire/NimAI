#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "AlphaBeta.h"

enum GAMETYPE { MISERE = 0, NORMAL = 1 };
enum AITYPE { ALGO = 0, ALPHABETA = 1 };

struct Move {
	int pile;
	int amount;
};


class NimBoard {
public:
	std::vector<int> piles;
	NimBoard(std::vector<int>, int, AITYPE);
	void update(Move);
	int getNimSum();
	Move getNextMove(GAMETYPE, int);
	void print();
	bool isGameOver();
	bool isValidMove(const Move&);
private:
	// Variables
	AITYPE ai;
	int depth = 0;
	int maxObjects = 0;
	// Constants
	int min = std::numeric_limits<int>::min();			// minimum/maximum values
	int max = std::numeric_limits<int>::max();
	// Functions
	Move getRandomMove();
	bool isEndGame();
	Tree* buildTree();
	int buildLayer(Node*, int);
	Move getMaxMove(Node*, int);
	int evaluator(std::vector<int>);
};