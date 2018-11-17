// Created  : Fri 16 Nov 2018 09:02:46 PM
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

// Number of Rows and Cols
// Standard Sudoku Grid allows 9x9
#define N 9

// Total number of cells
#define TOTAL_CELLS (N * N)

// Just a constant to indicate all cells are filled
#define GRID_FULL (-1)

////////////////////////////////////////////////////////////////
//                         IO stuffs                          //
////////////////////////////////////////////////////////////////

// print the grid
void print(int puzzle[]);

// input from stdin
void scan(int puzzle[], FILE *f);

////////////////////////////////////////////////////////////////
//                   Backtracking algorithm                   //
////////////////////////////////////////////////////////////////

// solve the puzzle. At least try to solve the puzzle.
bool solve(int puzzle[]);

////////////////////////////////////////////////////////////////
//                     Helper Functions                       //
////////////////////////////////////////////////////////////////

// get the closest empty cell
int get_next_empty_cell(int puzzle[]);

// checks if the value is in r-th row
bool in_row(int puzzle[], int r, int val);

// checks if the value is in c-th column
bool in_col(int puzzle[], int c, int val);

// checks if the value is in 3x3 box
bool in_3x3(int puzzle[], int r, int c, int val);

// checks if val is valid in p position
bool is_valid(int puzzle[], int p, int val);

// mapping 2D index to 1D index
int idx(int r, int c);

////////////////////////////////////////////////////////////////
//                          Main                              //
////////////////////////////////////////////////////////////////

int main(void) {
    // The actual grid
    int puzzle[TOTAL_CELLS] = {
        5,3,0,0,7,0,0,0,0,
        6,0,0,1,9,5,0,0,0,
        0,9,8,0,0,0,0,6,0,
        8,0,0,0,6,0,0,0,3,
        4,0,0,8,0,3,0,0,1,
        7,0,0,0,2,0,0,0,6,
        0,6,0,0,0,0,2,8,0,
        0,0,0,4,1,9,0,0,5,
        0,0,0,0,8,0,0,7,9
    };

    // Uncomment the line below to get input from stdin
    // scan(puzzle, stdin);

    print(puzzle);
    if (solve(puzzle)) {
        printf("Solved:\n");
        print(puzzle);
    } else {
        printf("Can't solve\n");
    }
    return 0;
}

////////////////////////////////////////////////////////////////
//                   Function Definitions                     //
////////////////////////////////////////////////////////////////

// get the closest empty cell
int get_next_empty_cell(int puzzle[]) {

    // Iterate through all the cells
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (puzzle[i] == 0)
            return i; // got an index
    }
    // We're here! Meaning grid is full and no empty cell left.
    return GRID_FULL;
}

// Solve the actual puzzle!!
bool solve(int puzzle[]) {

    // initialize a point
    int p;

    // get the nearest empty cell
    // if p equals to GRID_FULL constant we know we have solved the puzzle
    // we can return true in this case.
    if ((p = get_next_empty_cell(puzzle)) == GRID_FULL)
        return true;

    // Try values from 1 to 9 and backtrack until we get the result
    for (int i = 1; i < 10; ++i) {
        // check if i fits in p position
        if (is_valid(puzzle, p, i)) {
            // Yeah! 'i' is valid here!
            // set the value
            puzzle[p] = i;

            // The crucial recursive call!!
            if (solve(puzzle))
                return true;

            // reset the value to zero [backtracking]
            puzzle[p] = 0;
        }
    }
    // We couldn't solve the puzzle.
    // All the values are tested against the puzzle.
    // No luck. Return false
    return false;
}

// Check if the value 'val' is in r-th row
bool in_row(int puzzle[], int r, int val) {
    for (int i = 0; i < N; ++i) {
        if (puzzle[idx(r, i)] == val)
            return true;
    }
    return false;
}

// Check if the value 'val' is in c-th column
bool in_col(int puzzle[], int c, int val) {
    for (int i = 0; i < N; ++i) {
        if (puzzle[idx(i, c)] == val)
            return true;
    }
    return false;
}

// Check if the value 'val' is in 3x3 box
bool in_3x3(int puzzle[], int r, int c, int val) {
    for (int i = r; i < r + 3; ++i) {
        for (int j = c; j < c + 3; ++j) {
            if (puzzle[idx(i, j)] == val)
                return true;
        }
    }
    return false;
}

// Check if p position is a valid position
bool is_valid(int puzzle[], int p, int val) {

    // Trick to map 1D index to 2D index
    int c = p % N;
    int r = p / N;

    return !in_row(puzzle, r, val) && !in_col(puzzle, c, val) &&
           !in_3x3(puzzle, r - r % 3, c - c % 3, val);
}

// Trick to map 2D index to 1D index
int idx(int r, int c) { return r * N + c; }

// Simply print the puzzle.
void print(int puzzle[]) {
    // I didn't invest any time on printing.
    // Spaghetti code ahead!!!
    for (int i = 0; i < N; ++i) {
        if (i % 3 == 0)
            printf(" -------------------------\n");

        for (int j = 0; j < N + 1; ++j) {
            if (j == N) {
                printf(" |");
                break;
            }
            int val = puzzle[idx(i, j)];
            printf("%s%c", j % 3 == 0 ? " | " : " ",
                   val == 0 ? '.' : val + '0');
        }
        printf("\n");
    }
    printf(" -------------------------\n");
}

// Read from stdin
void scan(int puzzle[], FILE *f) {
    // Our 9x9 grid allows value from 1 to 9
    // So char is safe here!
    // Otherwise I had to deal with int or other type!
    char ch;
    int i = 0;
    while ((ch = getc(f)) != EOF && i < TOTAL_CELLS) {
        if (isdigit(ch)) {
            puzzle[i++] = ch - '0';
        }
    }
}
