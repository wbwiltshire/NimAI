#include "NimBoard.h"

using std::cout;
using std::endl;

// ctor
NimBoard::NimBoard(std::vector<int> p) : piles(p) {
	// Save the height of the biggest pile
	maxObjects = *std::max_element(std::begin(piles), std::end(piles));
}
// update the board with the move
void NimBoard::update(Move m) {
	piles[m.pile] -= m.amount;
}
int NimBoard::getNimSum() {
	int result = 0;

	for (int pile : piles)
		result ^= pile;		// Do a bit-wise XOR of the objects in each pile

	return result;
}
#pragma region getNextMove
Move NimBoard::getNextMove(GAMETYPE gameType, int nimSum) {
	Move move = { 0, 0 };
	int idx = 0;
	int targetSize = 0;
	int targetPile = 0;
	int removeAmount = 0;
	int pileMax = 0;
	bool isOdd = false;
	int movesLeft = 0;

	switch (gameType) {
	case MISERE:
		if (isEndGame()) {
			// Leave an odd number of piles of 1
			for (int p : piles)
				if (p > 0)
					movesLeft++;
			isOdd = (movesLeft % 2 == 1);
			pileMax = *std::max_element(std::begin(piles), std::end(piles));
			if (isOdd && (pileMax == 1)) {
				// No winning strategy, so generate random move
				move = getRandomMove();
			}
			else {
				auto it = std::find(piles.begin(), piles.end(), pileMax);	// file the pile with max
				move.pile = it - piles.begin();	// convert to an index
				move.amount = pileMax - (int)isOdd;
			}

		}
		else {
			for (int pile : piles) {
				targetSize = pile ^ nimSum;
				if (targetSize < pile) {
					removeAmount = pile - targetSize;
					move.pile = targetPile;
					move.amount = removeAmount;
					break;
				}
				else
					targetPile++;
			}
			// If we can't find an optimal move, return a random one
			if (removeAmount == 0)
				move = getRandomMove();
		}
		break;
	case NORMAL:
		for (int pile : piles) {
			targetSize = pile ^ nimSum;
			if (targetSize < pile) {
				removeAmount = pile - targetSize;
				move.pile = targetPile;
				move.amount = removeAmount;
				break;
			}
			else
				targetPile++;
		}
		// If we can't find an optimal move, return a random one
		if (removeAmount == 0)
			move = getRandomMove();
		break;
	}
	return move;
}
#pragma endregion
// Print the board to the screen
void NimBoard::print() {
	int count = 1;
	std::stringbuf sb;
	std::ostream os(&sb);		// assocate sb to output stream

	cout << "Board:" << endl;
	// Print the piles graphically
	for (int row = maxObjects; row > 0; row--) {
		for (int p = 0; p < piles.size(); p++) {
			if (piles[p] >= row)
				os << "** ";
			else
				os << "   ";
		}
		if ((int)sb.str().find("*") >= 0)
			cout << "\t" << sb.str() << endl;
		sb.str("");		//reset the stringbuffer
	}
	cout << "\t1  2  3  4" << endl;
}
bool NimBoard::isGameOver() {
	bool isOver = false;

	/// If all of the objects are gone, the game is over
	if (*std::max_element(std::begin(piles), std::end(piles)) == 0)
		isOver = true;

	return isOver;
}
bool NimBoard::isValidMove(const Move& m)
{
	if (m.amount < 1)
		return false;
	else if (m.amount <= piles[m.pile])
		return true;
	else
		return false;
}
// Helper function
Move NimBoard::getRandomMove() {
	Move move;
	int pile = -1;
	int amount = -1;

	cout << "Generating random move." << std::endl;

	while (true) {
		// pick a random pile between 0 and the number of piles
		pile = rand() % (int)piles.size();
		//Only leave, if we pick a pile with objects
		if (piles[pile] > 0)
			break;
	}

	move.pile = pile;
	// generate a random number between 1 and number of objects in the pile
	amount = 1 + (rand() % (int)piles[pile]);
	move.amount = amount > piles[pile] ? piles[pile] : amount;		// take amount if <= objects left

	return move;
}
// For Misere games, EndGame is when there are <= 1 piles of 2 or more objects
bool NimBoard::isEndGame() {
	int endGamePileCount = 0;
	bool isEndGame = false;
	// Return true, if all piles have 2 or less object left
	for (int p : piles)
	{
		if (p > 1)
			endGamePileCount++;
	}

	if (endGamePileCount <= 1)
		isEndGame = true;

	return isEndGame;
}
