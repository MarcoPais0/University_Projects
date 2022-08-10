#include <iostream>
#include <cstring>
#include <vector>
#include <map>

#define ROWS 50
#define COLUMNS 50
#define PIECES 2500

// Piece object
class Piece
{
public:
    int piece[4];
    bool used;

    // Piece dummy constructor
    Piece() {}

    // Piece constructor
    Piece(int p[4])
    {
        used = false;
        std::memcpy(piece, p, sizeof(piece));
    }

    // Rotates piece 90º
    void Rotate90()
    {
        int aux = piece[0];
        piece[0] = piece[3];
        piece[3] = piece[2];
        piece[2] = piece[1];
        piece[1] = aux;
    }

    // Rotates piece 180º
    void Rotate180()
    {
        int aux1 = piece[0];
        int aux2 = piece[1];
        piece[0] = piece[2];
        piece[1] = piece[3];
        piece[2] = aux1;
        piece[3] = aux2;
    }

    // Rotates piece 270º
    void Rotate270()
    {
        int aux = piece[3];
        piece[3] = piece[0];
        piece[0] = piece[1];
        piece[1] = piece[2];
        piece[2] = aux;
    }

    // Prints piece upper portion
    void printUp()
    {
        std::cout << piece[0] << " " << piece[1];
    }

    // Prints piece lower portion
    void printDown()
    {
        std::cout << piece[3] << " " << piece[2];
    }
};

// Board object
class Board
{
public:
    int board[ROWS][COLUMNS];
    int lines;
    int columns;
    Piece pieces[PIECES];

    // Board dummy constructor
    Board() {}

    // Board constructor
    Board(int l, int c, Piece p[])
    {
        lines = l - 1;
        columns = c - 1;
        std::memcpy(pieces, p, sizeof(Piece[l * c]));
    }

    // Recursive function to solve board
    bool solveBoard(int l, int c, std::map<std::array<int, 2>, std::vector<int>> *pairs)
    {
        std::vector<int> usable;
        if (c == columns)
        {
            usable = getPairValues(l, c, pairs);
            for (int i : usable)
            {
                if (!fits(&pieces[i], l, c, false))
                    continue;

                pieces[i].used = true;
                board[l][c] = i;

                if (l == lines)
                    return true;

                if (solveBoard(l + 1, 0, pairs))
                    return true;

                if (!fits(&pieces[i], l, c, true))
                {
                    pieces[i].used = false;
                    continue;
                }

                if (solveBoard(l + 1, 0, pairs))
                    return true;

                pieces[i].used = false;
            }
        }
        else
        {
            usable = getPairValues(l, c, pairs);
            for (int i : usable)
            {
                if (!fits(&pieces[i], l, c, false))
                    continue;

                pieces[i].used = true;
                board[l][c] = i;

                if (solveBoard(l, c + 1, pairs))
                    return true;

                if (!fits(&pieces[i], l, c, true))
                {
                    pieces[i].used = false;
                    continue;
                }

                if (solveBoard(l, c + 1, pairs))
                    return true;

                pieces[i].used = false;
            }
        }

        return false;
    }

    // Get pairs for a certain piece
    std::vector<int> getPairValues(int l, int c, std::map<std::array<int, 2>, std::vector<int>> *pairs)
    {
        if (l == 0 && c != 0)
        {
            // Check left
            if (pairs->find({pieces[board[l][c - 1]].piece[2], pieces[board[l][c - 1]].piece[1]}) != pairs->end())
                return pairs->find({pieces[board[l][c - 1]].piece[2], pieces[board[l][c - 1]].piece[1]})->second;
        }
        if (c == 0 && l != 0)
        {
            // Check up
            if (pairs->find({pieces[board[l - 1][c]].piece[3], pieces[board[l - 1][c]].piece[2]}) != pairs->end())
                return pairs->find({pieces[board[l - 1][c]].piece[3], pieces[board[l - 1][c]].piece[2]})->second;
        }
        if (c != 0 && l != 0)
        {
            // Check both
            if (pairs->find({pieces[board[l][c - 1]].piece[2], pieces[board[l][c - 1]].piece[1]}) != pairs->end())
                return pairs->find({pieces[board[l][c - 1]].piece[2], pieces[board[l][c - 1]].piece[1]})->second;
        }

        return {};
    }

    // Function to check piece fit
    bool fits(Piece *piece, int l, int c, bool flag)
    {
        if (!flag && piece->used)
            return false;

        // Check fit left
        if (l == 0 && c != 0)
            return CheckLeft(pieces[board[l][c - 1]].piece[1], pieces[board[l][c - 1]].piece[2], piece, flag);

        // Check fit up
        if (c == 0 && l != 0)
            return CheckUp(pieces[board[l - 1][c]].piece[2], pieces[board[l - 1][c]].piece[3], piece, flag);

        // Check fit up and left
        if (c != 0 && l != 0)
            return CheckBoth(pieces[board[l][c - 1]].piece[1], pieces[board[l][c - 1]].piece[2], pieces[board[l - 1][c]].piece[2], piece, flag);

        return false;
    }

    bool CheckLeft(int n1, int n2, Piece *p, bool flag)
    {
        // Flag is used for pieces that will try another rotation in the same fit
        if (!flag && n1 == p->piece[0] && n2 == p->piece[3])
            return true;

        if (n1 == p->piece[3] && n2 == p->piece[2])
        {
            p->Rotate90();
            return true;
        }

        if (n1 == p->piece[2] && n2 == p->piece[1])
        {
            p->Rotate180();
            return true;
        }

        if (n1 == p->piece[1] && n2 == p->piece[0])
        {
            p->Rotate270();
            return true;
        }

        return false;
    }

    bool CheckUp(int n1, int n2, Piece *p, bool flag)
    {
        // Flag is used to pieces that will try another rotation in the same fit
        if (!flag && n1 == p->piece[1] && n2 == p->piece[0])
            return true;

        if (n1 == p->piece[0] && n2 == p->piece[3])
        {
            p->Rotate90();
            return true;
        }

        if (n1 == p->piece[3] && n2 == p->piece[2])
        {
            p->Rotate180();
            return true;
        }

        if (n1 == p->piece[2] && n2 == p->piece[1])
        {
            p->Rotate270();
            return true;
        }

        return false;
    }

    bool CheckBoth(int n1, int n2, int n3, Piece *p, bool flag)
    {
        // Flag is used to pieces that will try another rotation in the same fit
        if (!flag && n1 == p->piece[0] && n2 == p->piece[3] && n3 == p->piece[1])
            return true;

        if (n1 == p->piece[3] && n2 == p->piece[2] && n3 == p->piece[0])
        {
            p->Rotate90();
            return true;
        }

        if (n1 == p->piece[2] && n2 == p->piece[1] && n3 == p->piece[3])
        {
            p->Rotate180();
            return true;
        }

        if (n1 == p->piece[1] && n2 == p->piece[0] && n3 == p->piece[2])
        {
            p->Rotate270();
            return true;
        }

        return false;
    }

    // Print board function
    void printBoard()
    {
        for (int i = 0; i < lines + 1; i++)
        {
            for (int j = 0; j < columns + 1; j++)
            {
                pieces[board[i][j]].printUp();
                if (j != columns)
                    std::cout << "  ";
                else
                    std::cout << std::endl;
            }

            for (int j = 0; j < columns + 1; j++)
            {
                pieces[board[i][j]].printDown();
                if (j != columns)
                    std::cout << "  ";
                else
                    std::cout << std::endl;
            }

            if (i != lines)
                std::cout << std::endl;
        }
    }
};

// Preprocesses pieces from board to optimize perfomance
bool preprocessing(std::map<std::array<int, 2>, std::vector<int>> *pairs, Piece pieces[PIECES], int size)
{
    int occurences[1000] = {0};
    int oddCount = 0;

    // Creates map with key -> pairs, value -> index of pieces with key pair
    pairs->insert(std::pair<std::array<int, 2>, std::vector<int>>({pieces[0].piece[1], pieces[0].piece[2]}, {0}));
    for (int i = 0; i < size; i++)
    {
        if (pairs->find({pieces[i].piece[0], pieces[i].piece[1]}) == pairs->end())
            pairs->insert(std::pair<std::array<int, 2>, std::vector<int>>({pieces[i].piece[0], pieces[i].piece[1]}, {i}));
        else
        {
            std::array<int, 2> aux = {pieces[i].piece[0], pieces[i].piece[1]};
            pairs->at(aux).push_back(i);
        }

        if (pairs->find({pieces[i].piece[1], pieces[i].piece[2]}) == pairs->end())
            pairs->insert(std::pair<std::array<int, 2>, std::vector<int>>({pieces[i].piece[1], pieces[i].piece[2]}, {i}));
        else
        {
            std::array<int, 2> aux = {pieces[i].piece[1], pieces[i].piece[2]};
            pairs->at(aux).push_back(i);
        }

        if (pairs->find({pieces[i].piece[2], pieces[i].piece[3]}) == pairs->end())
            pairs->insert(std::pair<std::array<int, 2>, std::vector<int>>({pieces[i].piece[2], pieces[i].piece[3]}, {i}));
        else
        {
            std::array<int, 2> aux = {pieces[i].piece[2], pieces[i].piece[3]};
            pairs->at(aux).push_back(i);
        }

        if (pairs->find({pieces[i].piece[3], pieces[i].piece[0]}) == pairs->end())
            pairs->insert(std::pair<std::array<int, 2>, std::vector<int>>({pieces[i].piece[3], pieces[i].piece[0]}, {i}));
        else
        {
            std::array<int, 2> aux = {pieces[i].piece[3], pieces[i].piece[0]};
            pairs->at(aux).push_back(i);
        }

        // Count the numbers that have an odd amount of occurrences
        for (int j = 0; j < 4; j++)
        {
            occurences[pieces[i].piece[j]]++;
            if (occurences[pieces[i].piece[j]] % 2 != 0)
                oddCount++;
            else
                oddCount--;
        }
    }

    // Check if puzzle is impossible
    if (oddCount > 4)
        return false;
    else
        return true;
}

int main()
{
    // Faster I/O
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    // Number of cases
    int cases, count = 0;
    std::cin >> cases;
    Board *boards = new Board[cases];
    while (count < cases)
    {
        // Puzzle specification
        int puzzle[3];
        for (int i = 0; i < 3; i++)
            std::cin >> puzzle[i];

        // Puzzle pieces
        Piece *pieces = new Piece[puzzle[0]];
        int auxPiece[puzzle[0]][4];
        for (int i = 0; i < puzzle[0]; i++)
        {
            // Reads piece
            for (int j = 0; j < 4; j++)
                std::cin >> auxPiece[i][j];

            // Adds new piece object to index i of array pieces
            pieces[i] = Piece(auxPiece[i]);
        }
        pieces[0].used = true;

        // New puzzle board added to boards
        boards[count] = Board(puzzle[1], puzzle[2], pieces);
        boards[count].board[0][0] = 0;
        count++;

        delete[] pieces;
    }

    // Solve all boards
    for (int i = 0; i < cases; i++)
    {
        std::map<std::array<int, 2>, std::vector<int>> pairs;
        if (preprocessing(&pairs, boards[i].pieces, (boards[i].lines + 1) * (boards[i].columns + 1)))
        {
            // Solves board
            if (boards[i].columns == 0)
            {
                if (boards[i].solveBoard(1, 0, &pairs))
                    boards[i].printBoard();
                else
                    std::cout << "impossible puzzle!" << std::endl;
            }
            else
            {
                if (boards[i].solveBoard(0, 1, &pairs))
                    boards[i].printBoard();
                else
                    std::cout << "impossible puzzle!" << std::endl;
            }
        }
        else
            std::cout << "impossible puzzle!" << std::endl;
    }
    delete[] boards;

    return 0;
}
