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
// struct car {
//   int row_c;  // row #
//   int col_c;  // column #
//   int is_changed; // T/F
// };

// A block to pass the arguments around the threads
struct block {
  WINDOW * win;   // window pointer
  // char msg[64];   // useless for now
  // struct car C;   // the car associated with the window
};

int flag = 0;     // useless for now

// void blink(int n, WINDOW* win, int cur_r, int cur_c);
void blink2(int n, WINDOW* win, int cur_r, int cur_c);
void* runner1(void *params);
void* runner2(void *params);
// void* runner3(void *params);
void* runner4(void *params);
void PrintCar(WINDOW * win);
void MakeBlank(WINDOW * win);

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
  struct block args1, args2, args3, args4;
  args1.win = inputwin;
  is_car_changed = 1;
  // args1.C.is_changed = 1;
  // args4.C = args1.C;
  args2.win = inputwin;
  args3.win = inputwin;
  args4.win = inputwin;
  // args2 = args1;
  pthread_t tid1, tid2, tid3, tid4;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  refresh();
  wrefresh(inputwin);
  MakeBlank(inputwin);
  pthread_create(&tid1, &attr, runner1, &args1);
  pthread_create(&tid2, &attr, runner2, &args2);
  // pthread_create(&tid3, &attr, runner3, &args3);
  pthread_create(&tid4, &attr, runner4, &args4);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  // pthread_join(tid3, NULL);
  pthread_join(tid4, NULL);
  
  getch();
  endwin();
  return 0;
}

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
      mvwprintw(win, j, i, " ");
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

// Doesn't do anything yet
void* runner3 (void *params) {

}

// listen to keyboard
void *runner4 (void *params) {
  struct block * args = params;
  while(1) {
    // usleep(4);
    // args->C.col_c + 30;
    // wrefresh(args->win);
    int c = wgetch(args->win);
    if (c == 'a') {
      // printf("yes");
      // wprintw(args->win, "hi");
      // memset(args->msg,0,strlen(args->msg));
      // strcpy(args->msg, "hi a!");
      // mvwprintw(args->win, 0, 0, args->msg);
      if (col_car > 10) {
        col_car -= 20;
        is_car_changed = 1;
      }
      // wrefresh(args->win);
    } 
    else if (c == 'd') {
      // memset(args->msg,0,strlen(args->msg));
      // strcpy(args->msg, "hi a!");
      // mvwprintw(args->win, 0, 0, args->msg);
      if (col_car < 40) {
        col_car += 20;
        is_car_changed = 1;
      }
      // wrefresh(args->win);
    } 
  }
  pthread_exit(NULL);
}


// void* runner1(void *params) {
//   struct block * args = params;
//   while(1) {
//     int c = wgetch(args->win);
//     if (c == 'a') {
//       memset(args->msg,0,strlen(args->msg));
//       strcpy(args->msg, "hi a!");
//       mvwprintw(args->win, 0, 0, args->msg);
//       wrefresh(args->win);
//     } 
//     else if (c == 'd') {
//       memset(args->msg,0,strlen(args->msg));
//       strcpy(args->msg, "hi d!");
//       mvwprintw(args->win, 0, 0, args->msg);
//       wrefresh(args->win);
//     } 
//     else if (c == 'w') {
//       memset(args->msg,0,strlen(args->msg));
//       // strcpy(msg, "hi w!");
//       mvwprintw(args->win, 0, 0, args->msg);
//       printf("hello world\n\r");
//       wrefresh(args->win);
//     } 
//     else if (c == 's') {
//       memset(args->msg,0,strlen(args->msg));
//       strcpy(args->msg, "hi s!");
//       wrefresh(args->win);
//     } 
//     else if (c == 'q') {
//       break;
//     }
//     else {
//       memset(args->msg,0,strlen(args->msg));
//       strcpy(args->msg, "Invalid input!");
//       mvwprintw(args->win, 0, 0, args->msg);
//       wrefresh(args->win);
//     } 
//   }
//   pthread_exit(NULL); 

// }


void blink2(int n, WINDOW* win, int cur_r, int cur_c) {
  wrefresh(win);
  // char str[] = "|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r";
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

// void blink(int n, WINDOW* win, int cur_r, int cur_c) {
//   wrefresh(win);
//   char str[] = "|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r";
//   // int cur_r = 1;
//   // int cur_c = 0;
//   if (n == 1) {
//     mvwprintw(win, cur_r, cur_c, "\n\r");
//     cur_r++;
//     // printf("hello\n");
//   }
//   mvwprintw(win, cur_r, cur_c, str);
//   return;
// }

