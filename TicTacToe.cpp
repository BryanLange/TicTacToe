#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

class TTT {
private:
    static const int N = 9; // size of board
    int board[N];
    int currentTurn;
    static const int BLANK = 1;
    static const int X = 2;
    static const int OH = 3;
public:
    TTT();
    ~TTT(){}
    void run();
    int playerMove();
    int computerMove(int turnNum);
    void display();
    void tileType(int x);
    void winStates(int i, int *a, int *b, int *c);
    bool isWinner(int player);
    int possWin(int player);
    int make2inRow(int player);
    void moveTo(int position);
    bool isFree(int choice);
    bool isFull();
    int getAnySpace();
    int getTurn();
    void switchTurn();
    void showRules();
    void reset();
}game; // object of class TTT created initially



int main()
{
    while(true) {
        char play;

        game.run();

        cout << "Would you like to play again?(y/n): ";
        cin >> play;
        if(play=='n' || play=='N') break;

        system("cls");
    }

    return 0;
} // end main()



TTT::TTT()
{ // constructor
    game.reset();
} // end constructor



void TTT::reset()
{ // resets the board to all blanks
    for(int i=0; i<N; i++) {
        board[i]=BLANK;
    }
    currentTurn=X;
} // end reset()



void TTT::run()
{ // runs the current game
    int position = -1; // last move made by either player
    game.reset();


    for(int turnNum=1; turnNum<=N; turnNum++) {
        game.display();


        // display computer's last move on player's turn
        // (position != -1) skips display on the first turn
        if((position != -1) && (game.getTurn()!=OH)) {
            cout << "Computer moved to tile: " << position+1 << endl;
        }


        // get the current player's move
        if(game.getTurn()==X) {
            position = game.playerMove();
        } else {
            position = game.computerMove(turnNum);
        }
        game.moveTo(position);



        // winner is found, game over
        if(game.isWinner(game.getTurn())) {
            system("cls");
            break;
        }


        game.switchTurn();
        system("cls");
    } // end for loop


    // display winning state
    game.display();
    if(game.isFull()) {
        cout << "It's a draw!" << endl;
    } else {
        cout << "Player " << ((currentTurn==X) ? 'X' : 'O') << " is the winner!" << endl;
    }
} // end run()



void TTT::display()
{ // displays a sample board with numbered tiles so the user knows where to move,
  // as well as the actual board and its current state
    game.showRules();

    cout << endl << "      Board Locations\t      Current Board" << endl; // headers for each board

    for(int i=0; i<N; i++) {
        if(i==0 || i==3 || i==6) {
            if(i!=0) {
            cout << "\t---+---+---\t       ---+---+---" << endl;    // displays horizontal separator part of game board
            }
            cout << "\t " << i+1 << " | " << i+2 << " | " << i+3 << "\t\t"; // displays the numbers on sample board
        }

        if(i==0 || i==1 || i==3 || i==4 || i==6 || i==7) {
            game.tileType(board[i]); cout << " |";  // displays blank, X, or O WITH a following separator
        }

        else if(i==2 || i==5 || i==8) {
            game.tileType(board[i]);                // displays blank, X, or O WITHOUT a following separator
        }

        cout << ((i%3==2) ? '\n' : ' ' );           // keeps displaying on same line until i/3 has a remainder of 2

    } // end for loop
    cout << endl;
} // end display()



void TTT::showRules()
{ // displays the rules of TicTacToe
    cout << "Welcome to Tic-Tac-Toe! (Human vs. Computer)" << endl;
    cout << " Be the first to get three in a row!" << endl;
    cout << "  The board locations are numbered 1-9." << endl;
    cout << "   You are Player X. The computer is Player O." << endl;
    cout << "    X goes first. Good luck!" << endl;
} // end showRules()



void TTT::tileType(int x)
{ // prints a blank, X, or O depending on the board state, used in display()
    switch(x) {
        case BLANK:
            cout << ' '; break;
        case X:
            cout << 'X'; break;
        case OH:
            cout << 'O'; break;
    }
} // end tileType()



int TTT::getTurn()
{ // returns which player's turn it is
    return currentTurn;
} // end getTurn()



int TTT::playerMove()
{ // prompts player to enter a position to move to

    int choice;

    // keeps asking until valid position is entered
    while(true) {
        cout << "Please make a move: ";
        cin >> choice;

        // break loop if choice is valid
        // choice-1 to match array position
        if((choice>=1) && (choice<=N) && (game.isFree(choice-1))) {
            break;
        }
        cout << "Invalid position" << endl;
    }

    return choice-1;
} // end getMove()



bool TTT::isFree(int choice)
{ // checks whether choice is BLANK
    return ((board[choice]==BLANK) ? true : false);
} // end isFree()



int TTT::computerMove(int turnNum)
{ // returns a move made by computer based on what turn number it is
    switch(turnNum) {

    // move to 4 if free, else move to 0
    case 2:
        if(isFree(4)) {
            return 4;
        } else {
            return 0;
        }

    // if player can win, block player
    // else make 2-in-row
    case 4:
        if(game.possWin(X) != -1) {
            return game.possWin(X);
        } else {
            return game.make2inRow(OH);
        }

    // if computer can win, win
    // else if player can win, block
    // else make 2-in-row
    case 6:
        if(game.possWin(OH) != -1) {
            return game.possWin(OH);
        } else if(game.possWin(X) != -1) {
            return game.possWin(X);
        } else {
            return game.make2inRow(OH);
        }

    // if computer can win, win
    // else if player can win, block
    // else get any free space
    case 8:
        if(game.possWin(OH)!= -1) {
            return game.possWin(OH);
        } else if(game.possWin(X)!= -1) {
            return game.possWin(X);
        } else {
            return game.getAnySpace();
        }

    } // end switch
} // end computerMove()



int TTT::possWin(int player)
{ // returns -1 if player cannot win
  // else returns the position player can win at

    // first, middle, and last of 3-in-row
    int first, middle, last;

    // checks all 8 winning states
    for(int i=0; i<8; i++) {

        // keeps track of what each location is in each 3-in-row combination
        int XinRow = 0, OinRow = 0, blankLocation = -1;

        // sets first, middle, and last values to one of the 3-in-row combinations(based on i)
        game.winStates(i, &first, &middle, &last);

        // record what tile first is
        switch(board[first]) {
        case BLANK: {blankLocation = first; break;}
        case X: {XinRow++; break;}
        case OH: {OinRow++; break;}
        }

        // record what tile middle is
        switch(board[middle]) {
        case BLANK: {blankLocation = middle; break;}
        case X: {XinRow++; break;}
        case OH: {OinRow++; break;}
        }

        // record what tile last is
        switch(board[last]) {
        case BLANK: {blankLocation = last; break;}
        case X: {XinRow++; break;}
        case OH: {OinRow++; break;}
        }


        // if current player can win, return blank location
        if(player==X) {
            if(XinRow==2 && blankLocation!=-1) {
                return blankLocation;
            }
        }
        else {
            if(OinRow==2 && blankLocation!=-1) {
                return blankLocation;
            }
        }

    } // end for loop
    return -1;
} // end possWin()



void TTT::winStates(int i, int *a, int *b, int *c)
{ // holds all 8 possible 3-in-a-row location combinations
    switch(i) {
    case 0: {*a=4; *b=1; *c=7; break;} // middle column
    case 1: {*a=4; *b=5; *c=3; break;} // middle row
    case 2: {*a=4; *b=2; *c=6; break;} // forward slash diagonal
    case 3: {*a=4; *b=0; *c=8; break;} // backslash diagonal
    case 4: {*a=0; *b=1; *c=2; break;} // top row
    case 5: {*a=0; *b=3; *c=6; break;} // left column
    case 6: {*a=8; *b=5; *c=2; break;} // right column
    case 7: {*a=8; *b=7; *c=6; break;} // bottom row
    }
} // end winStates()



int TTT::make2inRow(int player)
{ // returns position to make 2-in-row

    // first, middle, and last of 3-in-row combinations
    int first, middle, last;

    // loop checks all 8 winning states
    for(int i=0; i<8; i++) {

        // keeps track of what each location is in each 3-in-row
        int XinRow = 0, OinRow = 0, blankInRow = 0;

        // sets first, middle, and last values to one of the 3-in-a-row combinations(based on i)
        game.winStates(i, &first, &middle, &last);

        // record what tile first is
        switch(board[first]) {
        case BLANK: {blankInRow++; break;}
        case X: {XinRow++; break;}
        case OH: {OinRow++; break;}
        }

        // record what tile middle is
        switch(board[middle]) {
        case BLANK: {blankInRow++; break;}
        case X: {XinRow++; break;}
        case OH: {OinRow++; break;}
        }

        // record what tile last is
        switch(board[last]) {
        case BLANK: {blankInRow++; break;}
        case X: {XinRow++; break;}
        case OH: {OinRow++; break;}
        }

        if(player==X) {
            // if X has an X and 2 BLANKS in one of the 8 winning states
            // then return location next to the X
            if(XinRow==1 && blankInRow==2) {
                if(board[middle]==X) {
                    return first;
                } else {
                    return middle;
                }
            }
        }
        else { // if player OH
            // if OH has an OH and 2 BLANKS in one of the 8 winning states
            // then return location next to the OH
            if(OinRow==1 && blankInRow==2) {
                if(board[middle]==OH) {
                    return first;
                } else {
                    return middle;
                }
            }
        }
    } // end for loop
} // end make2inRow()



int TTT::getAnySpace()
{ // returns random free position on board
    int x;
    srand(time(NULL)); // random seed

    while(true) {
        x = rand() % N; // random board position
        if(game.isFree(x)) break;
    }
    return x;
} // end getAnySpace()



void TTT::moveTo(int position)
{ // moves player to selected position
    board[position] = ((game.getTurn()==X) ? X : OH);
} // end moveTo()



bool TTT::isWinner(int player)
{ // checks to see if player is a winner
    // first middle and last of a 3-in-a-row
    int first, middle, last;

    for(int i=0; i<8; i++) {
        // sets first, middle, and last values to one of the 3-in-a-row combinations(based on i)
        game.winStates(i, &first, &middle, &last);

        // if all three positions are equal to player type, then player is a winner
        if(board[first]==player && board[middle]==player && board[last]==player) {
            return true;
        }
    }
    return false;
} // end isWinner()



void TTT::switchTurn()
{ // switches turn to other player
    currentTurn = ((currentTurn==X) ? OH : X);
} // end switchTurn()



bool TTT::isFull()
{ // checks whether the board is full or not
    for(int i=0; i<N; i++) {
        // if there are blanks on the board, return false
        if(board[i]==BLANK) return false;
    }
    return true;
} // end isFull()
