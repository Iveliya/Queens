// QueensGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

const char EMPTY_CELL = '.';

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

int main()
{
    int rows = 7;
    int cols = 10;

    char** board = createBoard(rows, cols);
    printBoard(board, rows, cols);

    std::cout << "Test isInside:\n";
    std::cout << "(0,0): " << isInside(rows, cols, 0, 0) << std::endl;
    std::cout << "(-1,0): " << isInside(rows, cols, -1, 0) << std::endl;
    std::cout << "(rows,0): " << isInside(rows, cols, rows, 0) << std::endl;


    destroyBoard(board, rows);  
    return 0;
}


