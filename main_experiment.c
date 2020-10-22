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

#define T 1.8


// Coordinates of the CAR
struct car {
  int row_c;  // row #
  int col_c;  // column #
  int is_changed; // T/F
};

struct block {
  WINDOW * win;
  char msg[64];
  struct car C;
};

int flag = 0;

void blink(int n, WINDOW* win, int cur_r, int cur_c);
void blink2(int n, WINDOW* win, int cur_r, int cur_c);
void* runner1(void *params);
void* runner2(void *params);
void* runner3(void *params);
void* runner4(void *params);

int main() {
  initscr();
  noecho();
  cbreak();
  char msg[64];
  WINDOW * inputwin = newwin(25, 110, 1, 0);
  box(inputwin, 0, 0);
  refresh();
  wrefresh(inputwin);
  // struct car C;
  // C.row_c = 20;
  // C.col_c = 36;
  struct block args1, args2, args3, args4;
  args1.win = inputwin;
  args1.C.col_c = 50;
  args1.C.row_c = 16;
  args1.C.is_changed = 1;
  args4.C = args1.C;
  args2.win = inputwin;
  args3.win = inputwin;
  args4.win = inputwin;
  // args2 = args1;
  pthread_t tid1, tid2, tid3, tid4;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

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

// prints the car location
void* runner1(void *params) {
  struct block * args = params;
  // move(args->C.row_c, args->C.col_c);
  while(1) {
    if(args->C.is_changed == 1) {
      // wrefresh(args->win);
      mvwprintw(args->win, args->C.row_c, args->C.col_c, "****");
      mvwprintw(args->win, args->C.row_c + 1, args->C.col_c, "****");
      mvwprintw(args->win, args->C.row_c + 2, args->C.col_c, "****");
      mvwprintw(args->win, args->C.row_c + 3, args->C.col_c, "****");
      mvwprintw(args->win, args->C.row_c + 4, args->C.col_c - 4, "************");
      mvwprintw(args->win, args->C.row_c + 5, args->C.col_c - 4, "************");
      mvwprintw(args->win, args->C.row_c + 6, args->C.col_c - 4, "************");
      mvwprintw(args->win, args->C.row_c + 7, args->C.col_c - 4, "****    ****");
      mvwprintw(args->win, args->C.row_c + 8, args->C.col_c - 4, "****    ****");
      mvwprintw(args->win, args->C.row_c + 9, args->C.col_c - 4, "****    ****");
      args->C.is_changed = 0;
    }
  }
  // wprintw(args->win, " ***** ");
  pthread_exit(NULL);
}

// listen to keyboard
void *runner4 (void *params) {
  struct block * args = params;
  while(1) {
    // sleep(4);
    // args->C.col_c + 30;
    // wrefresh(args->win);
    int c = wgetch(args->win);
    if (c == 'a') {
      // printf("yes");
      wprintw(args->win, "hi");
      // memset(args->msg,0,strlen(args->msg));
      // strcpy(args->msg, "hi a!");
      // mvwprintw(args->win, 0, 0, args->msg);
      args->C.col_c -= 20;
      args->C.is_changed = 1;
      // wrefresh(args->win);
    } 
    else if (c == 'd') {
      // memset(args->msg,0,strlen(args->msg));
      // strcpy(args->msg, "hi a!");
      // mvwprintw(args->win, 0, 0, args->msg);
      args->C.col_c += 20;
      args->C.is_changed = 1;
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

// runs left boundary
void* runner2(void *params) {
  struct block * args = params;
  while(1) {
    blink2(0, args->win, 1, 0);
    sleep(T);
    blink2(1, args->win, 1, 0);
    sleep(T);  
  }
  printf ("Welcome to my game \n");
  pthread_exit(NULL);
}

// runs right boundary
void* runner3(void *params) {
  struct block * args = params;
  while(1) {
    blink2(0, args->win, 0, 90);
    sleep(T);
    blink2(1, args->win, 0, 90);
    sleep(T);  
  }
  printf ("Welcome to my game \n");
  pthread_exit(NULL);
}

void blink2(int n, WINDOW* win, int cur_r, int cur_c) {
  wrefresh(win);
  // char str[] = "|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r";
  int main = cur_r;
  int i = 26;
  while(i--) {
    mvwprintw(win, cur_r, cur_c, "||| ");
    cur_r++;
  }
  cur_r = main;
  if (n == 1) {
    mvwprintw(win, cur_r, cur_c, ",,,");
    cur_r++;
  }
  i = 26;
  while(i--) {
    mvwprintw(win, cur_r, cur_c, ",,,");
    cur_r += 3;
  }
  
  return;
}

void blink(int n, WINDOW* win, int cur_r, int cur_c) {
  wrefresh(win);
  char str[] = "|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r\n\r|||\n\r|||\n\r";
  // int cur_r = 1;
  // int cur_c = 0;
  if (n == 1) {
    mvwprintw(win, cur_r, cur_c, "\n\r");
    cur_r++;
    // printf("hello\n");
  }
  mvwprintw(win, cur_r, cur_c, str);
  return;
}

