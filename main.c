#include "solveur.h"

const char* testr = 
"053000900\
060015000\
000906300\
200000070\
401008090\
006007000\
500080000\
008400260\
040002080";

const char* test2 =
"005000080736000005020007030060071090000000800009230001071000008090000000608020300";

int main(void){
  clock_t t1;
  sudoku_p s = sudoku_new();
  sudoku_init(s, testr);
  sudoku_print(s);
  printf("\n\n====================== START ========================\n\n");
  t1 = clock();
  sudoku_solve(s);
  t1 = clock()-t1;
  printf("\n\n====================== STOP =========================\n\n");
  printf("Computation Time : %lf\n\n",(double) t1/ (double) CLOCKS_PER_SEC);
  sudoku_print(s);
 

  sudoku_reset(s);
  sudoku_init(s, test2);
  sudoku_print(s);
  printf("\n\n====================== START ========================\n\n");
  t1 = clock();
  sudoku_solve(s);
  t1 = clock()-t1;
  printf("\n\n====================== STOP =========================\n\n");
  printf("Computation Time : %lf\n\n",(double) t1/ (double) CLOCKS_PER_SEC);
  sudoku_print(s);

  sudoku_free(s);

  return EXIT_SUCCESS;
}
