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

void sudoku_reset(sudoku_p s){
  memset(s, 0, sizeof(sudoku_t));
}

void sudoku_print(sudoku_p s){
  grid_print(s);
  queue_print(s);
}

void grid_print(sudoku_p s){
  int i, j;
  for(i=0; i<9; i++){
    for(j=0; j<9; j++)
      printf("%u ",  GRID(s,i,j));
    
    printf("\n");
  }
}

void queue_print(sudoku_p s){
  char i, j;
  for(i=s->start; i<s->end; i++){
    printf("%u-[", s->queue[i]);
    for(j=1; j<10; j++)
      if(IS_POS(s, s->queue[i], j))
        printf("%u", j);
    printf("] ");
  }
  printf("\n");
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
  for(i=0; i<81; i++)
    s->grid[i] = str[i] - 48;

  if(queue_init(s)) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int queue_init(sudoku_p s){
  uchar nb, flag, temp;
  uchar i, j, k, x;
  uchar q, r, qi, qj;
  s->start = 0;
  s->end = 0;
  for(i=0; i<9; i++){
    for(j=0; j<9; j++){
      if(!GRID(s,i,j)){
        nb = 0;
        qi = i/3;
        qj = j/3;
        for(k=1; k<10; k++){
          flag = 1;
          for(x=0; (x<9) && flag; x++){
            q = x/3; r = x%3;
            /*column OR row OR square*/
            if ((GRID(s,x,j) == k) || 
                (GRID(s,i,x) == k) || 
                (GRID(s,(3*qi+q), (3*qj+r)) == k)){
              flag = 0;
            }
          }

          s->possible[9*i+j] += flag << (6+k);
          nb += flag;
        }

        if (!nb) return EXIT_FAILURE;

        s->possible[9*i+j] += nb;

        /*inserting in the queue*/
        flag = 9*i+j;
        for(k=s->start; k<s->end; k++){
          x = NB_POS(s, s->queue[k]);
          if (nb<x){
            temp = s->queue[k];
            s->queue[k] = flag;
            flag = temp;
            nb = x;
          }
        }

        s->queue[s->end] = flag;
        s->end ++;
      }
    }
  }

  return EXIT_SUCCESS;
}

int sudoku_solve(sudoku_p s){
  /*printf("%u, %u\n", QUEUE_FIRST(s), NB_BRANCH(s));*/
  uchar nb, flag;
  uchar i;
  sudoku_p c;

  if(DONE(s))
    return EXIT_SUCCESS;

  nb = NB_BRANCH(s);
  if(!nb)
    return EXIT_FAILURE;

  if(nb==1){
    for(i=1; i<10; i++)
      if(IS_POS(s, QUEUE_FIRST(s), i)){
        /*printf("(1)%u<-%u, ", QUEUE_FIRST(s), i);*/
        sudoku_update(s, i);
        return sudoku_solve(s);
      }
  }

  c = sudoku_copy(s);
  for(i=1; i<10; i++){
    if(IS_POS(s, QUEUE_FIRST(s), i)){
      /*printf("(%u)%u<-%u, ", nb, QUEUE_FIRST(s), i);*/
      sudoku_update(s,i);
      flag = sudoku_solve(s);
      if(flag)
        memcpy(s,c,sizeof(sudoku_t));
      else{
        sudoku_free(c);
        return EXIT_SUCCESS;
      }
    }
  }
  sudoku_free(c);

  return EXIT_FAILURE;
}

int sudoku_update(sudoku_p s, uchar x){
  uchar pos, p2;
  uchar k, i, j, q, r, qi, qj;
  pos = QUEUE_FIRST(s);

  i = pos/9; j = pos%9;
  qi = i/3; qj = j/3;

  /* update GRID and Start*/
  s->grid[pos] = x;
  s->start ++;

  for(k=0; k<9; k++){
    /*column*/
    p2 = 9*k+j;
    if(IS_POS(s, p2, x) && (p2 != pos)){
      /* POSSIBLE
       * shut down corresp bit + decreasing the counter*/
      s->possible[p2] -= (1<<(x+6)) + 1;

      queue_update(s, p2);
    }

    /*row*/
    p2 = 9*i+k;
    if(IS_POS(s, p2, x) && (p2 != pos)){
      s->possible[p2] -= (1<<(x+6)) + 1;

      queue_update(s, p2);
    }

    /*square*/
    q = k/3; r = k%3;
    p2 = 9*(3*qi+q)+3*qj+r;
    if(IS_POS(s, p2, x) && (p2 != pos)){
      s->possible[p2] -= (1<<(x+6)) + 1;

      queue_update(s, p2);
    }
  }
}

/* Use to upate the spot in the queue of the tile x when 
   its entropy have been decreased by one*/
int queue_update(sudoku_p s, uchar x){
  uchar xspot, spot, q;
  spot = s->start;
  for(q=s->start; q<s->end-1; q++){
    if(s->queue[q] == x){
      s->queue[q] = s->queue[spot];
      s->queue[spot] = x;
      return EXIT_SUCCESS;
    }

    if(Q_ENTROPY(s,q) < Q_ENTROPY(s,q+1))
      spot = q+1;
  }
  return EXIT_FAILURE;
}

char* sudoku_generate();
