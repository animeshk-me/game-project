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
#include<signal.h>

#define T 100000
#define SIZE 2

// Coordinates of the CAR
int row_car = 0;
int col_car = 0;
int is_car_changed = 0;
int game_over = 0;



int row_obs[SIZE];
int col_obs[SIZE];
int is_obs_changed[SIZE];


// A block to pass the arguments around the threads
struct block {
  WINDOW * win;   // window pointer
  int obs_num;    // obstacle number (0/1/2)
  int i_ptr;
  int j_ptr;
};

void blink_fence(int n, WINDOW* win, int cur_r, int cur_c);
void* runner1(void *params);
void* runner2(void *params);
void* runner3(void *params);
void* runner4(void *params);
void* runner5(void *params);
void* runner6(void* params);
void HandleHighscore(long long score, WINDOW * inputwin);
void PrintCar(WINDOW * win);
void MakeBlank(WINDOW * win);
void PrintObstacle(WINDOW * win, int row_c, int col_c);
void get_string(char * dest, long long src, int *dest_len);

int main() {
  initscr();
  noecho();
  cbreak();
  col_car = 30;
  row_car = 20;
  char msg[64];
  clock_t start, end;
  WINDOW * inputwin = newwin(26, 90, 1, 0);
  box(inputwin, 0, 0);

  struct block args1, args2, args3, args4, args5, args6;
  args1.win = inputwin;
  is_car_changed = 1;

  // initializing obstacles' coordinates
  for (int i = 0; i < SIZE; i++) { 
    row_obs[i] = 1;
    is_obs_changed[i] = 0;
  }
  args2.win = inputwin;
  args3.win = inputwin;
  args4.win = inputwin;
  args5.win = inputwin;
  args6.win = inputwin;
 
  pthread_t tid[6];
  refresh();
  wrefresh(inputwin);
  start = clock();        // record start time
  MakeBlank(inputwin);
  // Creating 6 detached threads
  pthread_create(&tid[0], NULL, runner1, &args1);
  pthread_create(&tid[1], NULL, runner2, &args2);
  pthread_create(&tid[2], NULL, runner3, &args3);
  pthread_create(&tid[3], NULL, runner4, &args4);
  pthread_create(&tid[4], NULL, runner5, &args5);
  pthread_create(&tid[5], NULL, runner6, &args6);
  for (int i = 0; i < 6; i++) {
    pthread_detach(tid[i]);
  }

  // Mechanism to take care of the 'GAME OVER' scenario
  while(1) {
    if(game_over) {
      end = clock();
      break;
    }
  }
  long long score = (end - start)/CLOCKS_PER_SEC;

  // Makes the screen blank
  for (int j = 0; j <= 26; j++) {
    for (int i = 0; i <= 64; i++) 
      mvwprintw(inputwin, j, i, " ");
  }

  mvwprintw(inputwin, row_car - 4, col_car - 2, "XXXXXXXXXXXXXXX");
  mvwprintw(inputwin, row_car - 3, col_car - 2, "XXX         XXX");
  mvwprintw(inputwin, row_car - 2, col_car - 2, "X XX       XX X");
  mvwprintw(inputwin, row_car - 1, col_car - 2, "X  XXx*\\/*xXX  X");
  mvwprintw(inputwin, row_car, col_car - 2, "X    X---X    X");
  mvwprintw(inputwin, row_car + 1, col_car - 2, "X  XX     XX  X");
  mvwprintw(inputwin, row_car + 2, col_car - 2, "X XX       XX X");
  mvwprintw(inputwin, row_car + 3, col_car - 2, "XXX         XXX");
  mvwprintw(inputwin, row_car + 4, col_car - 2, "XXXXXXXXXXXXXXX");
  sleep(1);

  // Makes the screen blank
  for (int j = 0; j <= 26; j++) {
    for (int i = 0; i <= 64; i++) 
      mvwprintw(inputwin, j, i, " ");
  }

  mvwprintw(inputwin, 10, 10, "//////-     /\\\\    |x\\___/x| XXXXXXX");
  mvwprintw(inputwin, 11, 10, "//         //_\\\\   |X-\\_/-X| XX____ ");
  mvwprintw(inputwin, 12, 10, "// /|//-  //|||\\\\  |X|   |X| XX^^^^ ");
  mvwprintw(inputwin, 13, 10, "//////-  //     \\\\ |X|   |X| XXXXXXX");
  usleep(10000);
  mvwprintw(inputwin, 15, 10, "XXXXXXX  \\\\     //  XXXXXXX  XXXx~\\     ");
  mvwprintw(inputwin, 16, 10, "XX   XX   \\\\   //   XX____   XX___))");
  mvwprintw(inputwin, 17, 10, "XX   XX    \\\\ //    XX^^^^   XXXx ");
  mvwprintw(inputwin, 18, 10, "XXXXXXX     \\\\//    XXXXXXX  XX  XX");

  mvwprintw(inputwin, 20, 20, "Your Score %d", score);
  HandleHighscore(score, inputwin);
  sleep(7);
  
  endwin();
  return 0;
}

// A utility which updates the highscore if needed
void HandleHighscore(long long score, WINDOW * inputwin) {
  int score_len = 0;
  FILE * fp = fopen("highscore_db.txt", "r+");
  char buffer[32];
  char score_str[64];
  // get the score in the string format
  get_string(score_str, score, &score_len);
  // read the highscore from file
  fgets(buffer, 32, fp);
  // get the highscore in integer format
  long long highscore = atoi(buffer);
  if(score > highscore) { // the case when new highscore happens
    freopen(NULL,"w+",fp);
    strcat(score_str, "\n");
    fwrite(score_str, score_len, 1, fp);
    mvwprintw(inputwin, 22, 18, "Congratulations! NEW High Score");
    mvwprintw(inputwin, 24, 18, "Last highscore %d smashed brutally", highscore);
  }
  fclose(fp);
}

// converts src to corresponding ascii string
void get_string(char * dest, long long src, int *dest_len) {
  *dest_len = 0;
  int temp;
  // length calculation
  int i = src;
  while(i != 0) {
    i /= 10;
    *dest_len = *dest_len + 1;  
  }
  // filling the passed string appropriately
  memset(dest, 0, SIZE);
  int j = *dest_len - 1;
  i = src;
  while(i != 0) {
    temp = i % 10;
    dest[j] = temp + 48;
    i = i / 10;
    j--;
  }
}


/*********** Printing the Car logic ********************/
// the thread which take care of printing the car logic based on changes 
void* runner1(void *params) {
  struct block * args = params;
  while(1) {
    if(is_car_changed == 1) {
      MakeBlank(args->win);
      PrintCar(args->win);
      is_car_changed = 0;
    }
  }
  pthread_exit(NULL);
}

// makes the whole screen blank except the obstacles
void MakeBlank(WINDOW * win) {
  for (int j = 0; j <= 26; j++) {
    for (int i = 3; i <= 60; i++) {
      for (int k = 0; k < SIZE; k++) {
        // usleep(10);
        if(!((j >= row_obs[k]) && (j <= row_obs[k]+3) && (i >= col_obs[k]) && (i <= col_obs[k]+6)))
          mvwprintw(win, j, i, " ");
      }
    }
  }
}

// Print the car at the given location
void PrintCar(WINDOW * win) {
  if(game_over)         // terminate the thread in case of game_over
    pthread_exit(NULL);
  mvwprintw(win, row_car, col_car, " ^**^ ");
  mvwprintw(win, row_car + 1, col_car, " **** ");
  mvwprintw(win, row_car + 2, col_car, "******");
  mvwprintw(win, row_car + 3, col_car - 5, " ************** ");
  mvwprintw(win, row_car + 4, col_car - 5, "*****^*****^****");
  mvwprintw(win, row_car + 5, col_car - 5, "****        ****");
}

/*********************************************************/


/*********************** Moving the Car logic *****************/
// this thread listens to the keyboard key presses for car movement
void *runner4 (void *params) {
  struct block * args = params;
  while(1) {
    if(game_over)
      break;
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

/******************* Fence movement logic ****************/

// the thread routine which runs both the fences
void* runner2(void *params) {
  struct block * args = params;
  while(1) {
    blink_fence(0, args->win, 1, 0);
    blink_fence(0, args->win, 1, 61);
    usleep(T);
    blink_fence(1, args->win, 1, 0);
    blink_fence(1, args->win, 1, 61);
    usleep(T);  
  }
  pthread_exit(NULL);
}

// a utility to make the fences blink 
void blink_fence(int n, WINDOW* win, int cur_r, int cur_c) {
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

/******************** Obstacle handling logic *********************/

// moves the obstacle 
void* runner3(void *params) {
  struct block * args = params;
  while(1) {
    for (int i = 0; i < SIZE; i++) {
      if(is_obs_changed[i] == 1) {
        mvwprintw(args->win, row_obs[i] - 1, col_obs[i] - 5, "                ");
        PrintObstacle(args->win, row_obs[i], col_obs[i]);
        usleep(99000);
        row_obs[i] = row_obs[i] + 1;
      }
    }
  }
  pthread_exit(NULL);
}

// generates the new obstacles periodically
void* runner5(void* params) {
  struct block * args = params;
  int i = 0;
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
    i = (i + 1) % SIZE;
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
      is_obs_changed[i] = 1;
      usleep(1901000);
      i = (i + 1) % SIZE;
    }
  }
  pthread_exit(NULL);
}

// print the obstacle at the passed row and column coordinate
void PrintObstacle(WINDOW * win, int row_c, int col_c) {
  if(game_over)
    pthread_exit(NULL);
  mvwprintw(win, row_c, col_c, "XXXXXX");
  mvwprintw(win, row_c + 1, col_c - 5, "/////XXXXXX\\\\\\\\\\");
  mvwprintw(win, row_c + 2, col_c - 3, "////XXXX\\\\\\\\");
  mvwprintw(win, row_c + 3, col_c, "\\~XX~/");
}

/*********************************************************/

/*********************** Hit obstacle logic *****************/

void* runner6(void *params) {
  struct block * args = params;
  while(1) {
    for (int i = 0; i < SIZE; i++) {
      if(col_obs[i] == col_car) {
        if((row_obs[i] >= row_car) && (row_obs[i] < 26)) {
          game_over = 1;
          break;
        }
      }
    }
  }
  pthread_exit(NULL);
}

/*********************************************************/




