#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum GAMETYPE { MISERE = 0, NORMAL = 1 };

struct Move {
	int pile;
	int amount;
};


class NimBoard {
public:
	std::vector<int> piles;
	NimBoard(std::vector<int>);
	void update(Move);
	int getNimSum();
	Move getNextMove(GAMETYPE, int);
	void print();
	bool isGameOver();
	bool isValidMove(const Move&);
private:
	// Variables
	int maxObjects = 0;
	Move getRandomMove();
	bool isEndGame();
};