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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// C includes
#include <time.h>

// Program include
#include "NimBoard.h"
#include "AlphaBeta.h"

// ENUMs
enum PLAYERTURN { PLAYER = 0, AI = 1 };

// Forward function declarations
void printMove(Move m, bool human = false);
void printGameType(GAMETYPE);
GAMETYPE getGameType();
PLAYERTURN getFirstTurn();
Move getHumanInput(NimBoard&);

using std::cin;
using std::cout;
using std::endl;

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

		system("cls");
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
#pragma region Helper functions

void printMove(Move m, bool human) {
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
#pragma endregion
