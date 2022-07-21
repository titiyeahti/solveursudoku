#ifndef H_SOLVEUR
#define H_SOLVEUR

#include <stdio.h>
#include <stdlib.h>
#include <integer.h>
#include <string.h>
#include <time.h>


/* grid, an array of 81 cell containing the current state of the sudoku
 * queue, the tiles to fill ordered by increasing entropy
 * start, end, indicate the indexes of start and end of the queue
 * possible, 
    + the nine upper bits (&511<<7) contains a bit field
      -> if i is a possible value for a tile T then possible[T] & (1<<i+6) == 1 
    + the 4 lower bits (&15) indicates how many possiblilities for the tile
    To summarize, if 1, 2 and 3 are the only possibilities for a tile T, 
    then possible[T] = 0b00000011 10000011
*/
typedef struct sudoku {
  uchar grid[81];
  uchar queue[81];
  ushort possible[81];
  char start, end;
} sudoku_t;

typedef sudoku_t* sudoku_p;

sudoku_p sudoku_new();

sudoku_p sudoku_copy(sudoku_p s);

void sudoku_free(sudoku_p s);

void sudoku_print(sudoku_p s);

void grid_print(sudoku_p s);
void queue_print(sudoku_p s);

int valid_string(const char* str);

int sudoku_init(sudoku_p s, const char* str);

int queue_init(sudoku_p s);

int sudoku_solve(sudoku_p sin, sudoku_p sout);

char* sudoku_generate();

#endif
