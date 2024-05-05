#include <iostream>
#include <vector>
#include <unordered_map>  
#define MAXINT 3
#define BOARDSIZE 20

// I Kurt Michael Postlmayr confirm that all code specific to the algorithm is hearby my work.
// I did use stackoverflow on marked locations for syntax purposes due to my unfamiliarity with the c++ language
// Macros I did know about before hand and can be changed above to make the board any size along with whatever random int range you want


using namespace std;

// Generates game's board via random integers for macro BOARDSIZE
vector<vector<int>> generateBoard()
{
    vector<vector<int>> arr(BOARDSIZE, vector<int>(BOARDSIZE, 0));
    for (int i = 0; i < BOARDSIZE; i++)
    {
        for(int j = 0; j < BOARDSIZE; j++)
            arr[i][j] = 1 + rand() % MAXINT;
    }
    return arr;
}

void printBoard(vector<vector<int>> board)
{
    for (int i = 0; i < BOARDSIZE; i++)
    {
        for(int j = 0; j < BOARDSIZE; j++)
        {
            if (board[i][j] != 0)
                cout << board[i][j] << " ";
            else
                cout << "  ";
        }
        cout << "\n";
    }       
}

// Used within isValid and outside for separate purpose
// More literal check if col and row input are within array's size
// We could use the size function from vector but I was unaware until late and the code it already written with the macro
bool isOutOfBounds(int row, int col)
{
    if (col >= BOARDSIZE || row >= BOARDSIZE || col < 0 || row < 0)
        return true;
    return false;
}

// Checks if the row col is a valid "move". Used both by users selection and computer
bool isValid(vector<vector<int>> board, int row, int col, int match)
{
    if (isOutOfBounds(row, col))
        return false;
    if (board[row][col] != match)
        return false;
    return true;
}

// Actual recursive function that checks all 4 directions (left right up down)
void recursiveSol(vector<vector<int>>& board, int row, int col, int match, unordered_map<int, int>& span)
{
    if (!isValid(board, row, col, match))
        return;

    // Gotta keep track of where you were to avoid endless recursion
    int storage = board[row][col];

    // Current position is valid because of base case therefore remove
    // So that we do not recursive over it causing infinite loop
    board[row][col] = 0;

    recursiveSol(board, row, col - 1, match, span); 
    recursiveSol(board, row, col + 1, match, span); 
    recursiveSol(board, row - 1, col, match, span); 
    recursiveSol(board, row + 1, col, match, span);

    // Falldown logic => due to order of recursive calls and base case we know for a fact:
    // 1) the current cell has been removed
    // 2) we are at the bottom of the column therefore everything that exists above needs to fall
    // therefore mark this position if it exists in the set as the furtherest down row in col
    // I had to look up unordered map syntax (ie functions it has) for below. Logic is mine though
    if (span.find(col) != span.end())
    {
        span[col] = (span[col] > row) ? span[col] : row;
    }
    else
    {
        span.insert({col, row});
    }
}

void drop(vector<vector<int>>& board, unordered_map<int, int> span)
{        
    // Iterate through span to drop
    // Syntax for iterating is not my work. Algorithm is my work
    for (const auto& pair : span)
    {
        //remember first is column in pair and second is row
        // Did not know unordered map class memebers prior to looking them up
        int i = pair.second;
        int tracker = pair.second;
        // The rest of this function is entirely my work
        while (i > 0)
        { 
            i--;
            if (board[i][pair.first] == 0)
                continue;
            board[tracker][pair.first] = board[i][pair.first];
            board[i][pair.first] = 0;
            tracker--; 
        } 
    }
}

// Wrapper for the games function. Basically play game/next move
void recursiveWrapper(vector<vector<int>> board)
{

    bool play = true;
    int userCol, userRow, match;
    printBoard(board); 

    do { 
        // Keeps track of columns that need to drop where first is column then second is row
        unordered_map<int, int> span;
        //cout << "Before deletion: \n";
        //printBoard(board);   
        cout << "Enter Column and Row or -1 to Quit: ";
        cin >> userCol >> userRow; 
        if (cin.fail()) 
        {
            cout << "Invalid row and column\n";
            cin.clear();
            continue;
        }
        if(userCol == -1)
            break;
        if(isOutOfBounds(userCol, userRow))
        {
            cout << "Invalid Column and Row";
            continue;
        }
        
        match = board[userRow][userCol];
        // Its already been deleted
        if (match == 0)
            continue;

        recursiveSol(board, userRow, userCol, match, span);
        // Print out everything since it's all been removed
        cout << "After deletion: \n";
        printBoard(board);

        drop(board, span);
        // Print out everything since it has now dropped
        cout << "\nAfter drop: \n";
        printBoard(board);

    } while (play == true);
    
}

int main()
{
    //srand(time(NULL));

    vector<vector<int>> board = generateBoard();
    recursiveWrapper(board);

    return 0;
}

