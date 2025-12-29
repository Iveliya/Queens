// QueensGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

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

int main()
{
    int rows = 5;
    int cols = 5;

    char** board = createBoard(rows, cols);

    printBoard(board, rows, cols);


    std::cout << "\nPlace P1 at (0,0):\n";
    if (!placeQueen(board, rows, cols, 0, 0, P1_QUEEN))
    {
        std::cout << "Invalid move!\n";
    }
    printBoard(board, rows, cols);

    std::cout << "\nPlace P2 at (0,2) (should be invalid because same row):\n";
    if (!placeQueen(board, rows, cols, 0, 2, P2_QUEEN))
    {
        std::cout << "Invalid move!\n";
    }
    printBoard(board, rows, cols);

    std::cout << "\nPlace P2 at (2,1):\n";
    if (!placeQueen(board, rows, cols, 2, 1, P2_QUEEN))
    {
        std::cout << "Invalid move!\n";
    }
    printBoard(board, rows, cols);
    



    destroyBoard(board, rows);  
    return 0;
}


