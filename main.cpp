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
enum PLAYERTURN { PLAYER = 0, AI = 1 };

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
	bool isValidMove(const Move& m)
	{
		if (m.amount < 1)
			return false;
		else if ( m.amount <= piles[m.pile])
			return true;
		else
			return false;
	}
private:
	// Variables
	int maxObjects = 0;

	// Helper function
	Move getRandomMove() {
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
	bool isEndGame() {
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

GAMETYPE getGameType() {
	bool validGameType = false;
	GAMETYPE gameType;
	int gameTypeChoice = 0;

	do {
		cout << "Enter game type(0=Misere, 1=Normal): ";
		cin >> gameTypeChoice;

		if (gameTypeChoice == 0 || gameTypeChoice == 1) {

			if (gameTypeChoice == 0)
				gameType = MISERE;
			else
				gameType = NORMAL;
			
			validGameType = true;
		}
		else
			cout << "Invalid choice, try again...." << endl;

	} while (!validGameType);

	return gameType;
}

PLAYERTURN getFirstTurn() {
	bool validPlayer = false;
	PLAYERTURN firstTurn;
	int playerChoice = 0;

	do {
		cout << "First to move(0=Player, 1=AI): ";
		cin >> playerChoice;

		if (playerChoice == 0 || playerChoice == 1) {

			if (playerChoice == 0)
				firstTurn = PLAYER;
			else
				firstTurn = AI;

			validPlayer = true;
		}
		else
			cout << "Invalid choice, try again...." << endl;

	} while (!validPlayer);

	return firstTurn;
}

Move getHumanInput(NimBoard& n) {
	Move move = { 0, 0 };
	int moveChoice = 0;
	bool validMove = false;

	do {
		cout << "Pile: ";
		cin >> moveChoice;

		if (moveChoice > 0 && moveChoice < 5) {
			move.pile = moveChoice - 1;

			cout << "Sticks: ";
			cin >> moveChoice;
			move.amount = moveChoice;

			if (!(validMove = n.isValidMove(move)))
				cout << "Invalid move, try again...." << endl;
		}
		else
			cout << "Invalid pile, try again...." << endl;

	} while (!validMove);

	return move;
}

int main(int argc, char* arv[])
{
	Move move;
	int nimSum = 0;
	GAMETYPE gameType;
	int returnCode = 0;
	PLAYERTURN turn;

	// Setup initial board and compute nimSum
	NimBoard nimBoard({ 1, 3, 5, 7 });

	// Seed random number generator
	srand(time(NULL));

    try
    {
		cout << "+-------------------------------------+" << endl;
		cout << "|           Welcome to NimAI!         |" << endl;
		cout << "+-------------------------------------+" << endl;
		
		gameType = getGameType();
		printGameType(gameType);
		turn = getFirstTurn();

		// Print the initial board and compute nimSum
		nimBoard.print();

		while (!nimBoard.isGameOver()) {

			if (turn == PLAYER) {
				move = getHumanInput(nimBoard);
				nimBoard.update(move);
				printMove(move, true);
				nimBoard.print();
				nimSum = nimBoard.getNimSum();
				cout << "nimSum: " << nimSum << std::endl;
				turn = AI;
			}
			else {
				// Generate the best next move
				move = nimBoard.getNextMove(gameType, nimSum);
				printMove(move);
				nimBoard.update(move);
				nimBoard.print();
				nimSum = nimBoard.getNimSum();
				cout << "nimSum: " << nimSum << std::endl;
				turn = PLAYER;
			}
		}

		if (turn == PLAYER && gameType == MISERE)
			cout << "\nGame over, Player has won!" << endl;
		else if (turn == AI && gameType == NORMAL)
			cout << "\nGame over, Player has won!" << endl;
		else
			cout << "\nGame over, AI has won!" << endl;
		cout << "\nPress <enter> to continue.. " << std::endl;
		cin.get();

    }
    catch(std::exception& e)
    {
        cout << e.what() << std::endl;
        returnCode = 1;
    }

	return returnCode;
}  