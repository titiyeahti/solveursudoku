#include "solveur.h"
#define SIZE 81
#define SIZE2 162

sudoku_p sudoku_new(){
  sudoku_p s = malloc(sizeof(sudoku_t));

  return s;
}

sudoku_p sudoku_copy(sudoku_p s){
  sudoku_p c = malloc(sizeof(sudoku_t));

  memcpy(c, s, sizeof(sudoku_t));

  return c;
}

void sudoku_free(sudoku_p s){
  free(s);
}

void sudoku_print(sudoku_p s){
  grid_print(s);
  queue_print(s);
}

void grid_print(sudoku_p s){
  int i, j;
  for(i=0; i<9; i++){
    for(j=0; j<9; j++)
      printf("%c ",  s->grid[9*i+j] != 0 ? (char) s->grid[9*i+j] : ' ');
    
    printf("\n");
  }
}

void queue_print(sudoku_p s){
  char i, j;
  for(i=s->start; i<s->end+1; i++){
    printf("%c-[", (char) s->queue[i]);
    for(j=1; j<10; j++)
      if(s->possible[i]&(1<<(j+6)))
        printf("%c", j);
    printf("] ");
  }
}

/* TODO 
   * check the len
   * check that there is only number between 0 and 9
   * check the integrity of rows, columns, and squares
*/
int valid_string(const char* str);

int sudoku_init(sudoku_p s, const char* str){
  /*if (valid_string(str)) return EXIT_FAILURE;*/
  int i;
  s->end = 0;
  for(i=0; i<81; i++){
    s->grid[i] = (uchar) str[i];
    s->end += s->grid[i] ? 1 : 0;
  }

  if(queue_init(s)) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int queue_init(sudoku_p s){
  uchar nb, flag;
  int i, j, k, x;
  int q, r, qi, ri, qj, rj;
  for(i=0; i<9; i++){
    for(j=0; j<9; j++){
      if(!s->grid[9*i+j]){
        nb = 0;
        for(k=1; k<10; k++){
          flag = 1;
          qi = i/3; ri = i%3;
          qj = j/3; rj = j%3;
          for(x=0; x<9 && flag; x++){
            /*column*/
            if ((i!=x) && (s->grid[9*x+j]) && 
                (s->grid[9*x+j] == s->grid[9*i+j]))
              flag = 0;

            /*row*/
            if ((j!=x) && (s->grid[9*i+x]) && 
                (s->grid[9*i+x] == s->grid[9*i+j]))
              flag = 0;

            q = x/3; r = x%3;
            /*square*/
            if ((q!=ri && r!=rj) && (s->grid[9*(3*qi+q)+3*qj+r]) &&
                (s->grid[9*(3*qi+q)+3*qj+r]) == s->grid[9*i+j])
              flag = 0;
          }

          s->possible[9*i+j] += flag << (6+k);
          nb += flag;
        }
        if (!nb) return EXIT_FAILURE;

        s->possible[9*i+j] += nb; 
      }
    }
  }

  return EXIT_SUCCESS;
}

int sudoku_solve(sudoku_p sin, sudoku_p sout){

}

char* sudoku_generate();
