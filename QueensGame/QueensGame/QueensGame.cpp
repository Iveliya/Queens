// QueensGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>


const char EMPTY_CELL = '.';
const char ATTACKED_CELL = '*';
const char P1_QUEEN = '1';
const char P2_QUEEN = '2';

char** createBoard(int rows, int cols)
{
    char** board = new char* [rows];

    for (int r = 0; r < rows; r++)
    {
        board[r] = new char[cols];

        for (int c = 0; c < cols; c++)
        {
            board[r][c] = EMPTY_CELL;
        }
    }

    return board;
}

void destroyBoard(char** board, int rows)
{
    if (board == NULL)
    {
        return;
    }

    for (int r = 0; r < rows; r++)
    {
        delete[] board[r];
    }

    delete[] board;
}

void printBoard(char** board, int rows, int cols)
{
    if (board == NULL)
    {
        std::cout << "(board is NULL)\n";
        return;
    }

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::cout << board[r][c] << ' ';
        }
        std::cout << '\n';
    }
}

bool isInside(int rows, int cols, int r, int c)
{
    if (r < 0 || r >= rows)
    {
        return false;
    }

    if (c < 0 || c >= cols)
    {
        return false;
    }

    return true;
}

bool isQueen(char cell)
{
    return cell == P1_QUEEN || cell == P2_QUEEN;
}

void clearAttackedMarks(char** board, int rows, int cols)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (board[r][c] == ATTACKED_CELL)
            {
                board[r][c] = EMPTY_CELL;
            }
        }
    }
}

void markDirection(char** board, int rows, int cols, int startR, int startC, int dR, int dC)
{
    int r = startR + dR;
    int c = startC + dC;

    while (isInside(rows, cols, r, c))
    {
        if (isQueen(board[r][c]))
        {
            return;
        }

        board[r][c] = ATTACKED_CELL;

        r += dR;
        c += dC;
    }
}

void recomputeAttackedCells(char** board, int rows, int cols)
{
    clearAttackedMarks(board, rows, cols);

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (isQueen(board[r][c]))
            {
                markDirection(board, rows, cols, r, c, -1, 0);
                markDirection(board, rows, cols, r, c, 1, 0);
                markDirection(board, rows, cols, r, c, 0, -1);
                markDirection(board, rows, cols, r, c, 0, 1);

                markDirection(board, rows, cols, r, c, -1, -1);
                markDirection(board, rows, cols, r, c, -1, 1);
                markDirection(board, rows, cols, r, c, 1, -1);
                markDirection(board, rows, cols, r, c, 1, 1);
            }
        }
    }
}

bool canPlaceQueenAt(char** board, int rows, int cols, int r, int c)
{
    if (!isInside(rows, cols, r, c))
    {
        return false;
    }

    if (board[r][c] != EMPTY_CELL)
    {
        return false;
    }

    const int dR[8] = { -1, 1, 0, 0, -1, -1,  1, 1 };
    const int dC[8] = { 0, 0,-1, 1, -1,  1, -1, 1 };

    for (int dir = 0; dir < 8; dir++)
    {
        int rr = r + dR[dir];
        int cc = c + dC[dir];

        while (isInside(rows, cols, rr, cc))
        {
            if (isQueen(board[rr][cc]))
            {
                return false;
            }

            rr += dR[dir];
            cc += dC[dir];
        }
    }

    return true;
}


bool placeQueen(char** board, int rows, int cols, int r, int c, char playerQueen)
{
    if (!canPlaceQueenAt(board, rows, cols, r, c))
    {
        return false;
    }

    board[r][c] = playerQueen;
    recomputeAttackedCells(board, rows, cols);
    return true;
}

void printHelp()
{
    std::cout << "Available commands:\n";
    std::cout << "  play r c   - place a queen at row r, column c\n";
    std::cout << "  show       - display the board\n";
    std::cout << "  turn       - show whose turn it is\n";
    std::cout << "  free       - list all valid moves for the current player\n";
    std::cout << "  help       - show this help message\n";
    std::cout << "  exit       - quit the game\n";
}

void printTurn(int currentPlayer)
{
    std::cout << "Current turn: Player " << currentPlayer << std::endl;
}

bool handlePlayCommand(char** board, int rows, int cols,
    int r, int c, int& currentPlayer)
{
    char queen = (currentPlayer == 1) ? P1_QUEEN : P2_QUEEN;

    if (!placeQueen(board, rows, cols, r, c, queen))
    {
        std::cout << "Invalid move!\n";
        return false;
    }

    currentPlayer = (currentPlayer == 1) ? 2 : 1;
    return true;
}

int countFreeCells(char** board, int rows, int cols)
{
    int count = 0;

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (canPlaceQueenAt(board, rows, cols, r, c))
            {
                count++;
            }
        }
    }

    return count;
}

void printFreeCells(char** board, int rows, int cols)
{
    int count = 0;

    std::cout << "Free cells:\n";

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (canPlaceQueenAt(board, rows, cols, r, c))
            {
                std::cout << "(" << r << "," << c << ") ";
                count++;
            }
        }

        if (count > 0)
        {
            std::cout << "\n";
        }
    }

    std::cout << "Total free: " << count << "\n";
}

bool isGameOver(char** board, int rows, int cols)
{
    return countFreeCells(board, rows, cols) == 0;
}

void printWinner(int currentPlayer)
{
    int winner = (currentPlayer == 1) ? 2 : 1;
    std::cout << "Game over! Player " << winner << " wins.\n";
}


void gameLoop(char** board, int rows, int cols)
{
    int currentPlayer = 1;
    char command[16];

    printHelp();
    printBoard(board, rows, cols);

    while (true)
    {
        if (isGameOver(board, rows, cols))
        {
            printWinner(currentPlayer);
            break;
        }

        std::cout << "\n> ";
        std::cin >> command;

        if (std::strcmp(command, "play") == 0)
        {
            int r, c;
            std::cin >> r >> c;
            handlePlayCommand(board, rows, cols, r, c, currentPlayer);
            printBoard(board, rows, cols);
        }
        else if (std::strcmp(command, "show") == 0)
        {
            printBoard(board, rows, cols);
        }
        else if (std::strcmp(command, "turn") == 0)
        {
            printTurn(currentPlayer);
        }
        else if (std::strcmp(command, "help") == 0)
        {
            printHelp();
        }
        else if (strcmp(command, "free") == 0)
        {
            printFreeCells(board, rows, cols);
        }
        else if (std::strcmp(command, "exit") == 0)
        {
            std::cout << "Exiting game...\n";
            break;
        }
        else
        {
            std::cout << "Unknown command. Type 'help' for options.\n";
        }
    }
}

int main()
{
    int rows = 5;
    int cols = 5;

    char** board = createBoard(rows, cols);

    gameLoop(board, rows, cols);
    
    destroyBoard(board, rows);  
    return 0;
}


