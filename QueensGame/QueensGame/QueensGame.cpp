// QueensGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
#include <fstream>



const char EMPTY_CELL = '.';
const char ATTACKED_CELL = '*';
const char P1_QUEEN = '1';
const char P2_QUEEN = '2';
const char MOVE_LOG_FILE[] = "moves_log.txt"; 
const char SAVES_FILE[] = "saves.txt";
const int MAX_SIZE = 15;

int readBoardSize(const char* name)
{
    int size;

    while (true)
    {
        std::cout << "Enter " << name << " (1.." << MAX_SIZE << "): ";
        std::cin >> size;

        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (size < 1 || size > MAX_SIZE)
        {
            std::cout << "Size must be between 1 and " << MAX_SIZE << ".\n";
            continue;
        }

        return size;
    }
}


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

void printMoveHistory(const char* filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cout << "No history available.\n";
        return;
    }

    char line[256];
    bool found = false;

    while (in.getline(line, 256))
    {
        if (std::strncmp(line, "Move ", 5) == 0)
        {
            std::cout << line << '\n';
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "(no moves yet)\n";
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
    std::cout << "  save name  - save current state with a name\n";
    std::cout << "  load name  - load state by name\n";
    std::cout << "  history    - show move history\n";


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

bool hasAnyValidMove(char** board, int rows, int cols)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (canPlaceQueenAt(board, rows, cols, r, c))
            {
                return true;
            }
        }
    }

    return false;
}

bool saveNamedGame(const char* filename, const char* saveName,
    char** board, int rows, int cols, int currentPlayer)
{
    std::ofstream out(filename, std::ios::app); 
    if (!out.is_open())
        return false;

    out << "SAVE " << saveName << "\n";
    out << rows << ' ' << cols << ' ' << currentPlayer << "\n";

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
            out << board[r][c];
        out << "\n";
    }

    out << "END\n\n";
    return true;
}


bool loadNamedGame(const char* filename, const char* saveName,
    char**& board, int& rows, int& cols, int& currentPlayer)
{
    std::ifstream in(filename);
    if (!in.is_open())
        return false;

    char word[32];
    char name[64];

    while (in >> word)
    {
        if (std::strcmp(word, "SAVE") != 0)
        {
            in.ignore(1024, '\n');
            continue;
        }

        in >> name;             
        in.ignore(1024, '\n');  

        int newRows, newCols, newPlayer;
        in >> newRows >> newCols >> newPlayer;
        in.ignore(1024, '\n');

        char** temp = createBoard(newRows, newCols);

        bool ok = true;
        for (int r = 0; r < newRows && ok; r++)
        {
            for (int c = 0; c < newCols; c++)
            {
                char ch = (char)in.get();
                if (!in || ch == '\n')
                {
                    ok = false;
                    break;
                }
                temp[r][c] = ch;
            }
            char endline = (char)in.get();
            if (!in || endline != '\n')
                ok = false;
        }

        char endWord[16];
        in >> endWord;
        in.ignore(1024, '\n');

        if (!ok || std::strcmp(endWord, "END") != 0)
        {
            destroyBoard(temp, newRows);
            return false;
        }

        if (std::strcmp(name, saveName) == 0)
        {
            destroyBoard(board, rows);
            board = temp;
            rows = newRows;
            cols = newCols;
            currentPlayer = newPlayer;

            recomputeAttackedCells(board, rows, cols);
            return true;
        }

        destroyBoard(temp, newRows);
    }

    return false; 
}


void resetMoveLog(const char* filename)
{
    std::ofstream out(filename); 
}

void appendMoveLog(const char* filename,
    int moveNumber, int player, int r, int c,
    char** board, int rows, int cols)
{
    std::ofstream out(filename, std::ios::app);
    if (!out.is_open())
    {
        return;
    }

    out << "Move " << moveNumber << ": Player " << player
        << " played (" << r << "," << c << ")\n";

    for (int rr = 0; rr < rows; rr++)
    {
        for (int cc = 0; cc < cols; cc++)
        {
            out << board[rr][cc] << ' ';
        }
        out << '\n';
    }

    out << "-------------------------\n";
}


void gameLoop(char**& board, int& rows, int& cols)
{

    int currentPlayer = 1;
    bool gameOver = false;
    char command[16];
    int moveNumber = 0;
    resetMoveLog(MOVE_LOG_FILE);


    printHelp();
    printBoard(board, rows, cols);

    while (!gameOver)
    {
        std::cout << "\n> ";
        std::cin >> command;

        if (std::strcmp(command, "play") == 0)
        {
            int r, c;
            std::cin >> r >> c;

            int playerWhoMoved = currentPlayer;

            if (handlePlayCommand(board, rows, cols, r, c, currentPlayer))
            {
                printBoard(board, rows, cols);


                moveNumber++;

                appendMoveLog(MOVE_LOG_FILE, moveNumber,
                    playerWhoMoved, r, c,
                    board, rows, cols);

                if (!hasAnyValidMove(board, rows, cols))
                {
                    std::cout << "No valid moves for Player "
                        << currentPlayer << "!\n";
                    std::cout << "Player "
                        << playerWhoMoved << " wins!\n";
                    gameOver = true;
                }
            }
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
        else if (std::strcmp(command, "free") == 0)
        {
            printFreeCells(board, rows, cols);
        }
        else if (std::strcmp(command, "history") == 0)
        {
            printMoveHistory(MOVE_LOG_FILE);
        }
        else if (std::strcmp(command, "exit") == 0)
        {
            std::cout << "Exiting game...\n";
            break;
        }
        else if (std::strcmp(command, "save") == 0)
        {
            char name[64];
            std::cin >> name;

            if (saveNamedGame(SAVES_FILE, name, board, rows, cols, currentPlayer))
                std::cout << "Saved as '" << name << "' in " << SAVES_FILE << "\n";
            else
                std::cout << "Save failed!\n";
        }
        else if (std::strcmp(command, "load") == 0)
        {
            char name[64];
            std::cin >> name;

            if (loadNamedGame(SAVES_FILE, name, board, rows, cols, currentPlayer))
            {
                std::cout << "Loaded '" << name << "' from " << SAVES_FILE << "\n";
                printBoard(board, rows, cols);
            }
            else
            {
                std::cout << "Load failed! No save named '" << name << "'.\n";
            }
        }
        else
        {
            std::cout << "Unknown command. Type 'help' for options.\n";
        }
    }
}


int main()
{
    int rows = readBoardSize("rows");
    int cols = readBoardSize("columns");

    char** board = createBoard(rows, cols);

    gameLoop(board, rows, cols);

    destroyBoard(board, rows);
    return 0;
}


