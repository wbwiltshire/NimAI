/****************************************************************************************************************/
/*                                                                                                              */ 
/*   NimAI: Game of Nim AI solutions                                                                            */
/*                                                                                                              */
/*   Misere rules: Player to take last object loses                                                             */
/*   Normal rules: Player to take last object wins																*/
/*                                                                                                              */
/*   Links: http://www.archimedes-lab.org/How_to_Solve/Win_at_Nim.html                                          */
/*          https://en.wikipedia.org/wiki/Nim#Mathematical_theory                                               */
/****************************************************************************************************************/

// System includes
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// C includes
#include <time.h>

using std::cin;
using std::cout;
using std::endl;

enum GAMETYPE { MISERE = 0, NORMAL = 1 };

struct Move {
	int pile;
	int amount;
};

#pragma region NimBoard
class NimBoard {
public:
	std::vector<int> piles;
    
	// ctor
	NimBoard(std::vector<int> p) : piles(p) {
		// Save the height of the biggest pile
		maxObjects = *std::max_element(std::begin(piles),std::end(piles));
	}
	
	// update the board with the move
	void update(Move m) {
		piles[m.pile] -= m.amount;
	}
	int getNimSum() {
		int result = 0;

		for (int pile : piles)
			result ^= pile;		// Do a bit-wise XOR of the objects in each pile

		return result;
	}
	Move getNextMove(GAMETYPE gameType, int nimSum) {
		Move move = { 0, 0 };
		int idx = 0;
		int targetSize = 0;
		int targetPile = 0;
		int removeAmount = 0;
		int pileMax = 0;

		switch (gameType) {
		case MISERE:
			if (isEndGame()) {
				// take 1 from the biggest pile
				pileMax = *std::max_element(std::begin(piles), std::end(piles));
				 auto it = std::find(piles.begin(), piles.end(), pileMax);
				 move.pile = it - piles.begin();	// convert to an index
				 move.amount = 1;
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
			move.pile = 2;
			move.amount = 2;
			break;
		}
		return move;
	}
	// Print the board to the screen
	void print() {
		int count = 1;
		std::stringbuf sb;
		std::ostream os(&sb);		// assocate sb to output stream
		
		cout << "Board:" << endl;
		// Print the piles graphically
		for (int row = maxObjects; row > 0; row--) {
			for (int p = 0; p < piles.size(); p++ ) {
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
	bool isGameOver() { 
		bool isOver = false;
		
		/// If all of the objects are gone, the game is over
		if (*std::max_element(std::begin(piles), std::end(piles)) == 0)
			isOver = true;
		
		return isOver;
	}
private:
	// Variables
	int maxObjects = 0;

	// Helper function
	Move getRandomMove() {
		Move move;
		int pile = -1;
		int object = -1;

		cout << "Generating random move." << std::endl;

		while (true) {
			// pick a random pile between 0 and the number of piles
			pile = rand() % ((int)piles.size() - 1);
			//Only leave, if we pick a pile with objects
			if (piles[pile] > 0)
				break;
		}

		move.pile = pile;
		// generate a random number between 1 and number of objects in the pile
		move.amount = 1 + (rand() % (int)piles[pile]);

		return move;
	}
	bool isEndGame() {
		bool isEndGame = false;

		// If all piles have 2 or less object left
		if (*std::max_element(std::begin(piles), std::end(piles)) <= 2)
			isEndGame = true;

		return isEndGame;
	}
};
#pragma endregion

void printMove(Move m, bool human = false) {
	if (human)
		cout << "\nYour move: \n\tRemove " << m.amount << " from pile " << m.pile + 1 << std::endl;
	else
		cout << "\nAI Move: \n\tRemove " << m.amount << " from pile " << m.pile + 1 << std::endl;
}
void printGameType(GAMETYPE gt) {
	if (gt == GAMETYPE::MISERE)
		cout << "Misere game style selected." << endl;
	else
		cout << "Normal game style selected." << endl;
}


int main(int argc, char* arv[])
{
	Move move;
	int nimSum = 0;
	GAMETYPE gameType = MISERE;
	int returnCode = 0;

	// Setup initial board and compute nimSum
	NimBoard nimBoard({ 1, 3, 5, 7 });

	// Seed random number generator
	srand(time(NULL));

    try
    {
		cout << "NimAI: " << std::endl;
		printGameType(gameType);

		// Print the initial board and compute nimSum
		nimBoard.print();
		nimSum = nimBoard.getNimSum();
		cout << "nimSum: " << nimSum << std::endl;

		// Reduce pile 4 by 2 and re-compute nimSum
		move.pile = 3; move.amount = 2;
		nimBoard.update(move);
		printMove(move, true);
		nimBoard.print();
		nimSum = nimBoard.getNimSum();
		cout << "nimSum: " << nimSum << std::endl;

		// Generate the best next move
		move = nimBoard.getNextMove(gameType, nimSum);
		printMove(move);
		nimBoard.update(move);
		nimBoard.print();
		nimSum = nimBoard.getNimSum();
		cout << "nimSum: " << nimSum << std::endl;

		// Reduce pile 3 by 3 and re-compute nimSum
		move.pile = 2; move.amount = 3;
		nimBoard.update(move);
		printMove(move, true);
		nimBoard.print();
		nimSum = nimBoard.getNimSum();
		cout << "nimSum: " << nimSum << std::endl;

		// Generate the best next move
		move = nimBoard.getNextMove(gameType, nimSum);
		printMove(move);
		nimBoard.update(move);
		nimBoard.print();
		nimSum = nimBoard.getNimSum();
		cout << "nimSum: " << nimSum << std::endl;

		// Reduce pile 3 by 2 and re-compute nimSum
		move.pile = 2; move.amount = 2;
		nimBoard.update(move);
		printMove(move, true);
		nimBoard.print();
		nimSum = nimBoard.getNimSum();
		cout << "nimSum: " << nimSum << std::endl;

		// Generate the best next move
		move = nimBoard.getNextMove(gameType, nimSum);
		printMove(move);
		nimBoard.update(move);
		nimBoard.print();
		nimSum = nimBoard.getNimSum();
		cout << "nimSum: " << nimSum << std::endl;

		// Reduce pile 1 by 1 and re-compute nimSum
		move.pile = 0; move.amount = 1;
		nimBoard.update(move);
		printMove(move, true);
		nimBoard.print();
		nimSum = nimBoard.getNimSum();
		cout << "nimSum: " << nimSum << std::endl;

		// Generate the best next move
		move = nimBoard.getNextMove(gameType, nimSum);
		printMove(move);
		nimBoard.update(move);
		nimBoard.print();

		if (nimBoard.isGameOver()) {
			cout << "\nGame over!" << endl;
		}

		cout <<  "\nPress <enter> to continue.. " << std::endl;
		cin.get();

    }
    catch(std::exception& e)
    {
        cout << e.what() << std::endl;
        returnCode = 1;
    }

	return returnCode;
}  