/* 
 * HW01_121044029_Mehmet_Gurol_Cay
 * main.cpp
 *
 * Created on September 20, 2015, 9:59 PM
 * 
 * Description
 * This program is a reversi game implementation. 
 * Human is 'O'. Computer is 'X'. 
 * Computer plays best move automatically. But I didn't implement min-max search
 * algorithm. Computer just does best move at that moment.
 */

#include <iostream>
#include <limits>

using namespace std;

const int CPCTY_OF_BOARD = 20;
const char HUMAN = 'O';
const char COMP = 'X';
const char NONE = '.';
const int FIX_COOR_VALUE = 64;
const char MIN_UPPER_CASE = 'A';
const char MAX_UPPER_CASE = 'Z';
const int MIN_LOWER_CASE = 97;
const int MAX_LOWER_CASE = 122;
const int FIX_CASE = 32;

bool checkValidBoard(int sizex, int sizey);
void prepBoard(int size, int board_arr[][CPCTY_OF_BOARD]);
void printOnScreen(int size, int board_arr[][CPCTY_OF_BOARD]);
void playGameFunc(int size, int board[][CPCTY_OF_BOARD]);

bool isValidMove(int size, int coorX, int coorY, char player,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);

int checkRightPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);
int checkLeftPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);
int checkUpPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);
int checkDownPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);
int checkRightDownPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);
int checkLeftDownPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);
int checkRightUpPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);
int checkLeftUpPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);

void bestMoveForComp(int size, int board[][CPCTY_OF_BOARD], char player);
void isThereAnyMove(int size, int board[][CPCTY_OF_BOARD], char player,
        bool *statusHuman, bool *statusComp);
int countBestMove(int size, int coorX, int coorY, char player,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard);

/*
 * Main function don't give any parameter on default or by user.
 */
int main(int argc, char** argv) {

    int board[CPCTY_OF_BOARD][CPCTY_OF_BOARD];
    int sizeX = 0, sizeY = 0;
    char temp;
    cout << "Enter the board size like this aXa: \n";

    cin >> sizeX >> temp >> sizeY;

    if (checkValidBoard(sizeX, sizeY) == true) {
        prepBoard(sizeX, board);
        playGameFunc(sizeX, board);

    } else {
        cout << "Invalid board size. Please enter the size like 4x4; 6x6; 8x8;";
        cout << " 10x10; ....; 20x20" << endl;
    }

    return 0;
}

/*
 * @param variableX     This parameter is size of X axis. X axis given by user.
 *                      This parameter is formal parameter.
 * @param variableY     This parameter is size of Y axis. Y axis given by user.
 *                      This parameter is formal parameter.
 * @description         This function check the sizes.If sizes aren't equal to 
 *                      between this function return false. If those are equal 
 *                      to between return true. But, if sizes are less than four
 *                      or more than twenty, there is no available size.
 */
bool checkValidBoard(int variableX, int variableY) {

    if (variableX == variableY) {
        if (variableX >= 4 && variableX % 2 == 0 && variableX <= 20) {
            return true;
        }
    }
    return false;
}

/*
 * @param size          This is size of board where the game is played.
 * @param boardArr      This array is for the game board. All arrangements will 
 *                      make by user.
 * @description         This function prepares the board for game. 
 */
void prepBoard(int size, int boardArr[][CPCTY_OF_BOARD]) {
    char stamp = 'X';
    int status = 0;

    for (int i = 0; i < size; ++i) {
        for (int k = 0; k < size; ++k) {
            boardArr[i][k] = NONE;
        }
    }

    for (int i = (size / 2) - 1; i <= size / 2; ++i) {
        for (int j = (size / 2) - 1; j <= size / 2; ++j) {
            boardArr[i][j] = stamp;
            if (status == 0)
                stamp = 'O';
            else if (status == 1)
                stamp = 'X';
        }
        status = 1;
    }

    return;
}

/*
 * @param size          This is size of board where the game is played.
 * @param boardArr      This array is for the game board. This array is  
 *                      two-dimensional.
 * @description         This function prints the board on the terminal.
 */
void printOnScreen(int size, int boardArr[][CPCTY_OF_BOARD]) {
    int line = 1;
    char column = 'A';

    cout << ' ';
    for (int i = 0; i < size; ++i) {
        cout << "|" << column;
        ++column;
    }
    cout << "|" << endl;

    for (int i = 0; i < size; ++i) {
        cout << line << "|";
        for (int k = 0; k < size; ++k) {
            cout << char (boardArr[i][k]) << "|";
        }
        cout << endl;

        ++line;
    }

    return;
}

/*
 * @param variable      It takes a character to be upper case. 
 * @description         The task of this function is to make capital letters and
 *                      return it.
 */

char toUpper(char variable) {
    return (static_cast<int> (variable) - FIX_CASE);
}

/*
 * @param size          This is size of board where the game is played.
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @description         The task of this function to start the game and to 
 *                      organize the game.
 */
void playGameFunc(int size, int board[][CPCTY_OF_BOARD]) {

    bool statusHuman = true,
            statusComp = true;
    char player = HUMAN;
    int coorY;
    char coorX, temp;
    bool sttCheckBoard = false;
    int humanMan = 0,
            compMan = 0;

    printOnScreen(size, board);

    while (statusHuman || statusComp) {
        isThereAnyMove(size, board, player, &statusHuman, &statusComp);
        if (player == HUMAN && statusHuman) {
            cout << "Your move. Please type coordinate: " << endl;
            cin >> coorY >> coorX;

            if ((0 < coorY && coorY <= 20) &&
                    (MIN_UPPER_CASE <= coorX && coorY <= MAX_UPPER_CASE)) {
                if (MIN_LOWER_CASE <= static_cast<int> (coorX) &&
                        static_cast<int> (coorX) <= MAX_LOWER_CASE) {
                    coorX = toUpper(coorX);
                }
                coorX = static_cast<char> (static_cast<int> (coorX) - FIX_COOR_VALUE);

                while (isValidMove(size, static_cast<int> (coorX), coorY, player, board, sttCheckBoard) == false) {
                    cout << "Your move isn't legal move. Please type again.\n";
                    cin >> coorY >> coorX;
                    if ((0 < coorY && coorY <= 20) &&
                            (MIN_UPPER_CASE <= coorX && coorY <= MAX_UPPER_CASE)) {
                        if (MIN_LOWER_CASE <= static_cast<int> (coorX) &&
                                static_cast<int> (coorX) <= MAX_LOWER_CASE) {
                            coorX = toUpper(coorX);
                            coorX = static_cast<char> (static_cast<int> (coorX) - FIX_COOR_VALUE);
                        }
                    } else
                        cout << "Invalid coordinate!! Please type again." << endl;
                }
                printOnScreen(size, board);
            } else
                cout << "Invalid coordinate!!" << endl;
        } else
            cout << "There isn't move for you." << endl;

        player = COMP;
        isThereAnyMove(size, board, player, &statusHuman, &statusComp);
        if (player == COMP && statusComp) {
            bestMoveForComp(size, board, player);
            printOnScreen(size, board);
        } else
            cout << "There isn't move for computer." << endl;

        player = HUMAN;
    }

    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            if (board[i][k] == HUMAN)
                ++humanMan;
            else if (board[i][k] == COMP)
                ++compMan;
        }
    }

    if (humanMan == compMan)
        cout << "DRAW!!" << endl;
    else if (humanMan < compMan)
        cout << "YOU LOSE!!" << endl;
    else if (compMan > humanMan)
        cout << "YOU WIN!!" << endl;
    else
        cout << "Something happened. :(" << endl;

    cout << "You: " << humanMan << endl << "Computer: " << compMan << endl;
    return;
}

/*
 * @param size          This is size of board where the game is played.
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param player        This parameter shows the who is playing to us.
 * @param *statusHuman  If there is any move for human this parameter gets true.
 *                      If there is no more any move this gets false.
 * @param *statusComp   If there is any move for computer this takes true.
 *                      If there is no more any move this gets false.
 * @description         This function checks the any move for both of player.
 *                      If there is any move this function return (with output
 *                      parameter) true for any player.
 */
void isThereAnyMove(int size, int board[][CPCTY_OF_BOARD], char player,
        bool *statusHuman, bool *statusComp) {

    bool sttComp = false;
    bool sttHuman = false;

    for (int i = 1; i <= size; i++) { //y
        for (int k = 1; k <= size; k++) { //x
            if (isValidMove(size, k, i, COMP, board, true) == true) {
                *statusComp = true;
                sttComp = true;
            } else if (sttComp != true)
                *statusComp = false;

        }
    }

    for (int i = 1; i <= size; i++) {
        for (int k = 1; k <= size; k++) {
            if (isValidMove(size, k, i, HUMAN, board, true) == true) {
                *statusHuman = true;
                sttHuman = true;
            } else if (sttHuman != true)
                *statusHuman = false;
        }
    }
}

/*
 * @param size          This is size of board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is the player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function is being used by isThereAnyMove function
 *                      sttCheckBoard parameter is true. Because, coordinate is
 *                      being tried by automatically. If this function is being
 *                      used by directly or by bestMoveForComp function 
 *                      sttCheckBoard is false. If function will finish 
 *                      successfully this function return true. If not, this 
 *                      return false.
 */
bool isValidMove(int size, int coorX, int coorY, char player,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    char oppPlayer;

    bool status = false;

    if (player == HUMAN)
        oppPlayer = COMP;
    else
        oppPlayer = HUMAN;

    //cout << "coorX: " << coorX << "coorY: " << coorY << endl;

    --coorX; //fixed for array index
    --coorY; //fixed for array index

    //cout << "fixed" << endl;
    //cout << "coorX: " << coorX << "coorY: " << coorY << endl;

    if (static_cast<char> (board[coorY][coorX]) == COMP ||
            static_cast<char> (board[coorY][coorX]) == HUMAN)
        return status;

    if ((0 <= coorX && 0 <= coorY) && (coorX < size && coorY < size)) {
        if (checkRightPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard) != 0)
            status = true;
        if (checkLeftPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard) != 0)
            status = true;
        if (checkUpPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard) != 0)
            status = true;
        if (checkDownPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard) != 0)
            status = true;
        if (checkRightDownPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard) != 0)
            status = true;
        if (checkLeftDownPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard) != 0)
            status = true;
        if (checkLeftUpPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard) != 0)
            status = true;
        if (checkRightUpPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard) != 0)
            status = true;

        return status;
    } else
        return status;
}

/*
 * @param size          This is size of board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param oppPlayer     This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function checks the right position is given to it. 
 */
int checkRightPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    int status = 0;
    int countOppPlayer = 0;

    for (int i = coorX + 1; i < size && status == 0; ++i) {
        if (static_cast<char> (board[coorY][i]) == oppPlayer)
            ++countOppPlayer;
        else if (static_cast<char> (board[coorY][i]) == player && countOppPlayer != 0)
            status = countOppPlayer;
        else
            return status;
    }

    if (status > 0 && sttCheckBoard != true) {
        for (int i = coorX, k = countOppPlayer; 0 <= k; i++, k--) {
            board[coorY][i] = static_cast<int> (player);
        }
    } else
        return status;
}

/*
 * @param size          This is size of board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param oppPlayer     This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function checks the left position is given to it. 
 */
int checkLeftPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    int status = 0;
    int countOppPlayer = 0;

    for (int i = coorX - 1; 0 <= i && status == 0; i--) {
        if (static_cast<char> (board[coorY][i]) == oppPlayer)
            ++countOppPlayer;
        else if (static_cast<char> (board[coorY][i]) == player && countOppPlayer != 0)
            status = countOppPlayer;
        else
            return status;
    }

    if (status > 0 && sttCheckBoard != true) {
        for (int i = coorX, k = countOppPlayer; 0 <= k; --i, --k) {
            board[coorY][i] = static_cast<int> (player);
        }
    } else
        return status;
}

/*
 * @param size          This is size of board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param oppPlayer     This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function checks the up position is given to it. 
 */
int checkUpPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    int status = 0;
    int countOppPlayer = 0;

    for (int i = coorY - 1; 0 <= i && status == 0; --i) {
        if (static_cast<char> (board[i][coorX]) == oppPlayer)
            ++countOppPlayer;
        else if (static_cast<char> (board[i][coorX]) == player && countOppPlayer != 0)
            status = countOppPlayer;
        else
            return status;
    }

    if (status > 0 && sttCheckBoard != true) {
        for (int i = coorY, k = countOppPlayer; 0 <= k; k--, i--) {
            board[i][coorX] = static_cast<int> (player);
        }
    } else
        return status;
}

/*
 * @param size          This is size of board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param oppPlayer     This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function checks the down position is given to it. 
 */
int checkDownPoss(int size, int coorX, int coorY, char player, char oppPlayer,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    int status = 0;
    int countOppPlayer = 0;

    for (int i = coorY + 1; i < size && status == 0; ++i) {
        if (static_cast<char> (board[i][coorX]) == oppPlayer)
            ++countOppPlayer;
        else if (static_cast<char> (board[i][coorX]) == player && countOppPlayer != 0)
            status = countOppPlayer;
        else
            return status;
    }

    if (status > 0 && sttCheckBoard != true) {
        for (int i = coorY, k = countOppPlayer; 0 <= k; i++, k--) {
            board[i][coorX] = static_cast<int> (player);
        }
    } else
        return status;
}

/*
 * @param size          This is size of board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param oppPlayer     This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function checks the right-down cross position is 
 *                      given to it. 
 */
int checkRightDownPoss(int size, int coorX, int coorY, char player,
        char oppPlayer, int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    int status = 0;
    int countOppPlayer = 0;

    for (int i = coorY + 1, k = coorX + 1; (i < size && k < size) && status == 0; i++, k++) {
        if (static_cast<char> (board[i][k]) == oppPlayer)
            ++countOppPlayer;
        else if (static_cast<char> (board[i][k]) == player && countOppPlayer != 0)
            status = countOppPlayer;
        else
            return status;
    }

    if (status > 0 && sttCheckBoard != true) {
        for (int i = coorY, k = coorX, j = countOppPlayer; 0 <= j; k++, i++, j--) {
            board[i][k] = static_cast<int> (player);
        }
    } else
        return status;
}

/*
 * @param size          This is size of board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param oppPlayer     This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function checks the left-up cross position is given
 *                      to it. 
 */
int checkLeftDownPoss(int size, int coorX, int coorY, char player,
        char oppPlayer, int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    int status = 0;
    int countOppPlayer = 0;

    for (int i = coorY + 1, k = coorX - 1; (0 <= k && i < size) && status == 0; k--, i++) {
        if (static_cast<char> (board[i][k]) == oppPlayer)
            ++countOppPlayer;
        else if (static_cast<char> (board[i][k]) == player && countOppPlayer != 0)
            status = countOppPlayer;
        else
            return status;
    }

    if (status > 0 && sttCheckBoard != true) {
        for (int i = coorY, k = coorX, j = countOppPlayer; 0 <= j; k--, i++, j--) {
            board[i][k] = static_cast<int> (player);
        }
    } else
        return status;
}

/*
 * @param size          This is size of board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param oppPlayer     This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function checks the right-up cross position is 
 *                      given to it. 
 */
int checkRightUpPoss(int size, int coorX, int coorY, char player,
        char oppPlayer, int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    int status = 0;
    int countOppPlayer = 0;

    for (int i = coorY - 1, k = coorX + 1; (k < size && 0 <= i) && status == 0; k++, i--) {
        if (static_cast<char> (board[i][k]) == oppPlayer)
            ++countOppPlayer;
        else if (static_cast<char> (board[i][k]) == player && countOppPlayer != 0)
            status = countOppPlayer;
        else
            return status;
    }

    if (status > 0 && sttCheckBoard != true) {
        for (int i = coorY, k = coorX, j = countOppPlayer; 0 <= j; i--, k++, j--) {
            board[i][k] = static_cast<int> (player);
        }
    } else
        return status;
}

/*
 * @param size          This is size of the board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param oppPlayer     This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function checks the left-up cross position is given 
 *                      to it. 
 */
int checkLeftUpPoss(int size, int coorX, int coorY, char player,
        char oppPlayer, int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {
    int status = 0;
    int countOppPlayer = 0;

    for (int i = coorY - 1, k = coorX - 1; (0 <= i && 0 <= k) && status == 0; i--, k--) {
        if (static_cast<char> (board[i][k]) == oppPlayer)
            ++countOppPlayer;
        else if (static_cast<char> (board[i][k]) == player && countOppPlayer != 0)
            status = countOppPlayer;
        else
            return status;
    }

    if (status > 0 && sttCheckBoard != true) {
        for (int i = coorY, k = coorX, j = countOppPlayer; 0 <= j; k--, i--, j--) {
            board[i][k] = static_cast<int> (player);
        }
    } else
        return status;
}

/*
 * @param size          This is size of the board where the game is played.
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param player        This is any player who computer(X) or human(O).
 * @description         This function give any coordinate countBestMove 
 *                      function and calculate the move that will larger eating 
 *                      man position. After that, this function save this 
 *                      positions.
 */
void bestMoveForComp(int size, int board[][CPCTY_OF_BOARD], char player) {
    char oppPlayer;
    int bestBeat = 0, temp = 0, tempBestCoorX = -1, tempBestCoorY = -1;


    if (player == HUMAN)
        oppPlayer = COMP;
    else
        oppPlayer = HUMAN;

    for (int y = 1; y <= size; y++) {
        for (int x = 1; x <= size; x++) {
            temp = countBestMove(size, x, y, player, board, true);
            if (bestBeat < temp) {
                bestBeat = temp;
                temp = 0;
                tempBestCoorX = x;
                tempBestCoorY = y;

            }
        }
    }

    isValidMove(size, tempBestCoorX, tempBestCoorY, player, board, false);
    cout << "Computer move is: " << tempBestCoorY
            << static_cast<char> (tempBestCoorX + FIX_COOR_VALUE) << endl;
}

/*
 * @param size          This is size of the board where the game is played.
 * @param coorX         This is x axis that is the place of played.
 * @param coorY         This is y axis that is the place of played.
 * @param player        This is any player who computer(X) or human(O).
 * @param board         This array is for the game board. This array is 
 *                      two-dimensional.
 * @param sttCheckBoard If the board is being checked sttCheckBoard is true.
 *                      If the board isn't being checked sttCheckBoard is false.
 * @description         This function calculate to eaten man. And return this.
 */
int countBestMove(int size, int coorX, int coorY, char player,
        int board[][CPCTY_OF_BOARD], bool sttCheckBoard) {

    int countBest = 0;
    char oppPlayer;

    if (player == HUMAN)
        oppPlayer = COMP;
    else
        oppPlayer = HUMAN;

    --coorX; //fixed for array index
    --coorY; //fixed for array index

    if (board[coorY][coorX] == NONE) {
        countBest += checkDownPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard);
        countBest += checkUpPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard);
        countBest += checkRightPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard);
        countBest += checkLeftPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard);
        countBest += checkLeftDownPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard);
        countBest += checkLeftUpPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard);
        countBest += checkRightDownPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard);
        countBest += checkRightUpPoss(size, coorX, coorY, player, oppPlayer, board, sttCheckBoard);
    }

    return countBest;
}