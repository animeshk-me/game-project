#include<stdio.h> 
#include<stdlib.h> 
#include<time.h>
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<pthread.h>
#include<fcntl.h>
#include<errno.h> 
#include<ncurses.h>

#define SIZE 64

struct block {
  int s1;
  int s2;
};

int length = 0;

void get_string(char * dest, long long src);

int main() {
  pthread_t tid1, tid2;
  struct block args1, args2;
  args1.s1 = 10;
  args2.s1 = 13;
  char buffer[32];
  char score_str[SIZE];
  FILE * fp = fopen("highscore_db.txt", "r+");
  srand(time(0));
  long long my_score = rand() % 10000000;
  printf("Your score %d\n", my_score);
  get_string(score_str, my_score);
  printf("Your score str = %s\n", score_str);
  fgets(buffer, 32, fp);
  // printf("file content %s\n", buffer);
  long long int f_cont = atoi(buffer);
  printf("file integer = %lld buffer = %s\n", f_cont, buffer);
  if(my_score > f_cont) {
    // printf("yeap \n");
    // fprintf(fp, "pagal");
    freopen(NULL,"w+",fp);
    strcat(score_str, "\n");
    fwrite(score_str, length, 1, fp);
    // fputs(score_str, fp);
  }
  return 0;
}

// converts src to corresponding ascii string
void get_string(char * dest, long long src) {
  int len = 0;
  int temp;
  int i = src;
  while(i != 0) {
    i /= 10;
    len++;  // length calculation
  }
  length = len;
  memset(dest, 0, SIZE);
  int j = len - 1;
  i = src;
  while(i != 0) {
    temp = i % 10;
    dest[j] = temp + 48;
    i = i / 10;
    j--;
    // len++;  // length calculation
  }
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