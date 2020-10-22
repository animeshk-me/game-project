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

struct block {
  int s1;
  int s2;
};

void boundary();
void blink(int n);

int main() {
  boundary();
  return 0;
}

void boundary() {
  while(1) {
    sleep(1);
    blink(0);
    sleep(1);
    blink(1);
  }
}

void blink(int n) {
  char str[] = "|||\n|||\n\n|||\n|||\n\n|||\n|||\n\n|||\n|||\n\n|||\n|||\n\n|||\n|||\n\n|||\n|||\n\n|||\n|||\n\n|||\n|||\n\n|||\n|||\n";
  if (n == 1)
    printf("\n");
  printf("%s", str);
  return;
}