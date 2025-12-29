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

int main()
{
    int rows = 5;
    int cols = 6;

    char** board = createBoard(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << board[i][j] << " ";
        }std::cout << std::endl;
    }

    destroyBoard(board, rows);   
    return 0;
}


