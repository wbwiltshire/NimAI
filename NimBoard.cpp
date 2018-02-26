#include "NimBoard.h"

using std::cout;
using std::endl;

// Forward function declarations
std::string makeNodeName(std::vector<int>);

// ctor
NimBoard::NimBoard(std::vector<int> p, int d, AITYPE a) : piles(p), depth(d), ai(a) {
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
		switch (ai) {
			case ALGO:
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
			case ALPHABETA:
				int max = 0;
				Tree* tree = buildTree();
				Node* root = tree->nodes();
				max = alphaBeta(root, depth, min, max, true);
				move = getMaxMove(root, max);
				delete root;
				break;
		}
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
Tree* NimBoard::buildTree() {
	Tree* tree;
	int treeSize = 0;
	Node* root = new Node("root", min);
	root->piles = piles;

	treeSize = buildLayer(root, depth);

	tree = new Tree(root);
	tree->size(treeSize);
	return tree;
}
int NimBoard::buildLayer(Node* parent, int level) {
	std::vector<int> newPile;
	Node* node = nullptr;
	int pile = 0;
	int nodes = 0;

	if (level == 0) {
		return 0;
	}
	else {
		// Build one ply / layer
		for (int value : parent->piles) {
			for (int remove = value; remove > 0; remove--)
			{
				newPile = parent->piles;
				newPile[pile] = newPile[pile] - remove;
				node = parent->AddChild(makeNodeName(newPile), max, newPile);
				node->value = evaluator(newPile);
				nodes += buildLayer(node, level - 1);
				nodes++;
			}
			pile++;

		}
	}
	return nodes;
}
Move NimBoard::getMaxMove(Node* n, int m) {
	Move move = { 0, 0 };
	Node* maxChild = nullptr;
	int idx = 0;

	// Find the first child maching the max
	for (Node* c : n->children) {
		if (c->value == m) {
			maxChild = c;
			break;
		}
	}
	// The next move is the in the pile whose value doesn't match
	for (int p : piles) {
		if (p != maxChild->piles[idx]) {
			move.pile = idx;
			move.amount = p - maxChild->piles[idx];
			break;
		}
		idx++;
	}

	return move;
}
int NimBoard::evaluator(std::vector<int> piles) {
	int result = 0;
	int nimSum = 0;

	//Compute the NimSum
	for (int pile : piles)
		nimSum ^= pile;

	// If last move, then AI won
	if (*std::max_element(std::begin(piles), std::end(piles)) == 0)
		result = 100;
	else if (nimSum == 0)
		result = 50;
	else
		result = min;

	return result;
}
//Helper functions
std::string makeNodeName(std::vector<int> list) {
	std::stringbuf sb;
	std::ostream os(&sb);		// assocate sb to output stream

	for (int l : list) {
		os << l << "-";
	}
	return sb.str().substr(0, sb.str().length() - 1);
}