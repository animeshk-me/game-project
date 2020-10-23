#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h> 
#include<ncurses.h>

#define T 100000


// Coordinates of the CAR
int row_car = 0;
int col_car = 0;
int is_car_changed = 0;

int row_obs[3];
int col_obs[3];
int is_obs_changed[3];


// A block to pass the arguments around the threads
struct block {
  WINDOW * win;   // window pointer
  int obs_num;    // obstacle number (0/1/2)
};


// void blink(int n, WINDOW* win, int cur_r, int cur_c);
void blink2(int n, WINDOW* win, int cur_r, int cur_c);
void* runner1(void *params);
void* runner2(void *params);
void* runner3(void *params);
void* runner4(void *params);
void* runner5(void *params);
void* runner6(void* params);

void PrintCar(WINDOW * win);
void MakeBlank(WINDOW * win);
void PrintObstacle(WINDOW * win, int row_c, int col_c);
// void BlankPrev(WINDOW * win);

int main() {
  initscr();
  noecho();
  cbreak();
  col_car = 30;
  row_car = 16;
  char msg[64];
  WINDOW * inputwin = newwin(26, 64, 1, 0);
  box(inputwin, 0, 0);
  // struct car C;
  // C.row_c = 20;
  // C.col_c = 36;
  struct block args1, args2, args3, args4, args5, args6;
  args1.win = inputwin;
  is_car_changed = 1;
  for (int i = 0; i < 3; i++) { 
    row_obs[i] = 1;
    is_obs_changed[i] = 0;
  }
  // args1.C.is_changed = 1;
  // args4.C = args1.C;
  args2.win = inputwin;
  args3.win = inputwin;
  args4.win = inputwin;
  args5.win = inputwin;
  args6.win = inputwin;
  // args2 = args1;
  pthread_t tid1, tid2, tid3, tid4, tid5, tid6;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  refresh();
  wrefresh(inputwin);
  MakeBlank(inputwin);
  pthread_create(&tid1, &attr, runner1, &args1);
  pthread_create(&tid2, &attr, runner2, &args2);
  pthread_create(&tid3, &attr, runner3, &args3);
  pthread_create(&tid4, &attr, runner4, &args4);
  pthread_create(&tid5, &attr, runner5, &args5);
  // pthread_create(&tid6, &attr, runner6, &args6);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
  pthread_join(tid4, NULL);
  pthread_join(tid5, NULL);
  // pthread_join(tid6, NULL);
  
  getch();
  endwin();
  return 0;
}

/*********** Printing the Car logic ********************/
// prints the car 
void* runner1(void *params) {
  struct block * args = params;
  // move(row_car, args->C.col_c);
  while(1) {
    if(is_car_changed == 1) {
      MakeBlank(args->win);
      PrintCar(args->win);
      is_car_changed = 0;
    }
  }
  pthread_exit(NULL);
}

void MakeBlank(WINDOW * win) {
  for (int j = 0; j <= 26; j++) {
    for (int i = 3; i <= 60; i++) {
      for (int k = 0; k < 3; k++) {
        if(!((j >= row_obs[k]) && (j <= row_obs[k]+3) && (i >= col_obs[k]) && (i <= col_obs[k]+6)))
          mvwprintw(win, j, i, " ");
      }
    }
  }
}

void PrintCar(WINDOW * win) {
  mvwprintw(win, row_car, col_car, "******");
  mvwprintw(win, row_car + 1, col_car, "******");
  mvwprintw(win, row_car + 2, col_car, "******");
  mvwprintw(win, row_car + 3, col_car, "******");
  mvwprintw(win, row_car + 4, col_car - 5, "****************");
  mvwprintw(win, row_car + 5, col_car - 5, "****************");
  mvwprintw(win, row_car + 6, col_car - 5, "****************");
  mvwprintw(win, row_car + 7, col_car - 5, "*****      *****");
  mvwprintw(win, row_car + 8, col_car - 5, "*****      *****");
  mvwprintw(win, row_car + 9, col_car - 5, "*****      *****");
}

/*********************************************************/

/******************* Fence movement logic ****************/

// runs both the fences
void* runner2(void *params) {
  struct block * args = params;
  while(1) {
    blink2(0, args->win, 1, 0);
    blink2(0, args->win, 1, 61);
    usleep(T);
    blink2(1, args->win, 1, 0);
    blink2(1, args->win, 1, 61);
    usleep(T);  
  }
  printf ("Welcome to my game \n");
  pthread_exit(NULL);
}

// blinks the fences
void blink2(int n, WINDOW* win, int cur_r, int cur_c) {
  wrefresh(win);
  int main = cur_r;
  int i = 24;
  while(i--) {
    mvwprintw(win, cur_r, cur_c, "||| ");
    cur_r++;
  }
  cur_r = main;
  if (n == 1) {
    mvwprintw(win, cur_r, cur_c, ",,,");
    cur_r++;
  }
  i = 23;
  while(i--) {
    mvwprintw(win, cur_r, cur_c, ",,,");
    cur_r += 3;
  }
  
  return;
}

/*********************************************************/

/******************** Obstacle logic *********************/

// moves the obstacle 
void* runner3(void *params) {
  struct block * args = params;

  while(1) {  
    // int i = 0;
    for (int i = 0; i < 3; i++) {
      if(is_obs_changed[i] == 1) {
        mvwprintw(args->win, row_obs[i] - 1, col_obs[i], "      ");
        // BlankPrev(args->win);
        PrintObstacle(args->win, row_obs[i], col_obs[i]);
        usleep(100000);
        row_obs[i] = row_obs[i] + 1;
        // is_obs_changed = 0;
      }
    }
  }
  pthread_exit(NULL);
}

// generates the new obstacles periodically
void* runner5(void* params) {
  struct block * args = params;
  int i = 0;
  // wprintw(args->win, "shul");
  while(1) {
    srand(time(0));
    int t = rand() % 2;
    if ((row_obs[i] >= 26) || (row_obs[i] == 1)){
      row_obs[i] = 1;
      col_obs[i] = col_car;
      is_obs_changed[i] = 1;
      usleep(1901000);
      continue;
    }
    i = (i + 1) % 3;
    if ((row_obs[i] >= 26) || (row_obs[i] == 1)){
      row_obs[i] = 1;
      if(col_car == 10) {
        switch(t) {
          case 0:
            col_obs[i] = 30;
            break;
          case 1:
            col_obs[i] = 50;
            break;
        }
      }
      else if(col_car == 30) {
        switch(t) {
          case 0:
            col_obs[i] = 50;
            break;
          case 1:
            col_obs[i] = 10;
            break;
        }
      }
      else if(col_car == 50) {
        switch(t) {
          case 0:
            col_obs[i] = 10;
            break;
          case 1:
            col_obs[i] = 30;
            break;
        }
      }
      // col_obs = 10;
      is_obs_changed[i] = 1;
      usleep(1901000);
      i = (i + 1) % 3;
      // col_obs[i] = car_location;
      // is_obs_changed[i] = 1;
      // i = (i + 1) % 3;
      // sleep(0.9);
      // while(row_obs[i] != 26) {}
    }
  }
  pthread_exit(NULL);
}


// print the obstacle
void PrintObstacle(WINDOW * win, int row_c, int col_c) {
  mvwprintw(win, row_c, col_c, "XXXXXX");
  mvwprintw(win, row_c + 1, col_c, "XXXXXX");
  mvwprintw(win, row_c + 2, col_c, "XXXXXX");
  mvwprintw(win, row_c + 3, col_c, "XXXXXX");
}

/*********************************************************/

/*********************** Moving the Car logic *****************/
// listen to keyboard
void *runner4 (void *params) {
  struct block * args = params;
  while(1) {
    int c = wgetch(args->win);
    if (c == 'a') {
      if (col_car > 10) {
        col_car -= 20;
        is_car_changed = 1;
      }
    } 
    else if (c == 'd') {
      if (col_car < 40) {
        col_car += 20;
        is_car_changed = 1;
      }
    } 
  }
  pthread_exit(NULL);
}

/**************************************************************/


