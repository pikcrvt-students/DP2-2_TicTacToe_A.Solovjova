#include <iostream>
#include <cstring>
#include <algorithm>
#include <ctime>

using namespace std;

#define compareBoxes(box1, box2, box3) ((board[box1] == board[box2]) && (board[box2] == board[box3]) && (board[box1] != 0)) //Pārbauda, vai trīs vienumi ir vienādi, un pārliecinās, ka tie nav 0.
#define numberToLetter(x) ((x > 0) ? (x == 1) ? 'X' : 'O' : ' ') //Paņem ciparu un pārvērš to par burtu vai atstarpi.

int getWinner(int board[9]) {
	//Atrod spēles uzvarētāju, ja uzvarētāja nav, atgriež 0.
	int winner = 0;
	for (int x = 0; x < 3; x++) {
		if (compareBoxes(3*x, 3*x+1, 3*x+2)) { //Pārbauda rindas.
			winner = board[3*x];
			break;
		} else if (compareBoxes(x, x+3, x+6)) { //Pārbauda kolonnas.
			winner = board[x];
			break;

		} else if (compareBoxes(2*x, 4, 8-2*x) && (x < 2)) { //Pārbauda diagonāles. 
			winner = board[4];
			break;
		}
	}
	return winner;
}
bool gameOver(int board[9]){
	//Pārbauda, vai spēle ir beigusies, un paziņo, kurš uzvarēja, vai neizšķirts.
	int winner = getWinner(board);
	if (winner > 0) {
		cout << numberToLetter(winner) << " wins!"<< endl;
		return true;
	} 
	for (int x = 0; x < 9; x++) {
		if (board[x] == 0) return false;
	}
	cout << "Tie!\n\n";
	return true;
}

int willWin(int board[9], int player) {
	//Pārbauda, vai konkrētais spēlētājs varētu uzvarēt nākamajā plankā.
	for (int x = 0; x < 9; x++) {
		int tempBoard[9];
		memcpy(tempBoard, board, 36); //memcy - Kopē baitu skaita vērtības no avota norādītās vietas tieši uz atmiņas bloku, uz kuru norāda galamērķis.
		if (board[x] > 0) continue;
		tempBoard[x] = player;
		if(getWinner(tempBoard) == player) return x;
	}
	return -1;
}

int exceptionalCase(int board[9]) {
	//Atrod apmales, kas ir algoritma darbības izņēmumi.
	int cases[2][9] = {{1,0,0,0,2,0,0,0,1}, {0,1,0,1,2,0,0,0,0}}; //Dēļi, kas nedarbojas ar algoritmu.
	int answers[2][4] = {{3,3,3,3}, {2,8,6,0}};
	int rotatedBoard[9] = {6,3,0,7,4,1,8,5,2};
	int newBoard[9];
	int tempBoard[9];
	for(int x = 0; x < 9; x++) {
		newBoard[x] = board[x];
	}
	for (int caseIndex = 0; caseIndex < 2; caseIndex++) {
		for(int rotation = 0; rotation < 4; rotation++) {
			for (int x = 0; x < 9; x++) 
				tempBoard[x] = newBoard[x];
			
			int match = 0;
			//Pagriež dēli, lai tas darbotos ar dažādām vienas un tās pašas plates versijām.
			for (int box = 0; box < 9; box++) {
				newBoard[box] = tempBoard[rotatedBoard[box]];
			}

			for (int x = 0; x < 9; x++) {
				if (newBoard[x] == cases[caseIndex][x]) match++;
				else break;
			}
			if (match == 9) return answers[caseIndex][rotation];
		}
	}
	return -1;
}

int getSpace(int board[9], int spaces[4]) {
	//Iegūst nejaušu stūri vai malu, kas nav aizņemta.
	bool isSpaceEmpty = false;
	int y;
	for (int x = 0; x < 4; x++) {
		if (board[spaces[x]] == 0) {
			isSpaceEmpty = true;
			break;
		}
	}
	if (isSpaceEmpty) {
		do {
			y = rand() % 4;
		} while (board[spaces[y]] != 0);
		return spaces[y];
	}
	return -1;
}

void outputBoard(int board[9]) {
	for(int line = 0; line < 3; line++){
		for (int box = 0; box < 3; box++) {
			cout << numberToLetter(board[3*line+box]) << ((box < 2) ? '|' : '\n');
		}
		cout << ((line < 2) ? "-----\n" : "\n");
	}
}

int main(){
	int board[9] = {0,0,0,0,0,0,0,0,0}; //Sāk tukšu dēli.
	int possibleWinner;
	int move;
	bool isInvalid;
	string moveString;
	srand((int) time(0));
	int corners[4] = {0,2,6,8};
	int sides[4] = {1,3,5,7};

	cout << "1|2|3\n-----\n4|5|6\n-----\n7|8|9\n\n";

	while (true) {
		//Spēlētājs X izlemj, kādu kustību viņš veiks.
		do {
			cout << "X: ";
			getline(cin, moveString);
			move = moveString[0] - '1';
			if (move > 8 || move < 0 || board[move] != 0) {
				cout << "Invalid input" << endl;
				isInvalid = true;
			} else {
				board[move] = 1;
				isInvalid = false;
				cout << endl;
			}
		} while (isInvalid);

		//Izlemj, vai spēle turpināsies vai ne.
		if (gameOver(board) > 0) {
			outputBoard(board);
			break;
		}

		//Spēlētājs O izlemj, kuru gājienu viņš veiks.
		bool good = false;
		for (int x = 2; x > 0; x--){
			possibleWinner = willWin(board, x);
			if (possibleWinner != -1) {
				board[possibleWinner] = 2;
				good = true;
				break;
			}
		}
		if (good);
		else if (board[4] == 0) board[4] = 2; //Vidus.
		else if (exceptionalCase(board) > -1) board[exceptionalCase(board)] = 2; //Izņēmuma dēļi.
		else if (getSpace(board, corners) != -1) board[getSpace(board, corners)] = 2; //Stūri
		else board[getSpace(board, sides)] = 2; //puses

		outputBoard(board);

		if(gameOver(board)) break;

	}
	return 0;
}
