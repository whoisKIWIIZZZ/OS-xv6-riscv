#include "kernel/types.h"
#include "user/user.h"

int busy = 1;
int sl = 2;

int
main(int argc, char *argv[])
{
  int state, test_mode, i, pid;
  int busy_pid[busy];
  int nchild;

  state = 0;
  test_mode = 0;

  if(argc >= 2){
    if(strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "-t") == 0)
      test_mode = 1;
    else
      state = atoi(argv[1]);
  }
  else{
    printf("usage:pstest [123]\n");
    exit(-1);
  }
  if(test_mode){
    nchild = sl+busy;

  
    printf("=== baseline ===\n");
    printf("Running:  %d\n", getprocn(1));
    printf("Runnable: %d\n", getprocn(2));
    printf("Sleeping: %d\n", getprocn(3));


    for(i = 0; i < sl; i++){
      if(fork() == 0){
        sleep(50);
        exit(0);
      }
    }

    for(i = 0; i < busy; i++){
      pid = fork();
      if(pid == 0){
        while(1);  
      }
      busy_pid[i] = pid;
    }

    sleep(5);
    printf("=== with test processes:4 while loop, 2 sleep loop ===\n");
    printf("Running:  %d\n", getprocn(1));
    printf("Runnable: %d\n", getprocn(2));
    printf("Sleeping: %d\n", getprocn(3));

    for(i = 0; i < busy; i++)
      kill(busy_pid[i]);

    // 6. wait for all children to exit
    for(i = 0; i < nchild; i++)
      wait(0);

    // 7. final snapshot
    printf("=== after cleanup ===\n");
    printf("Running:  %d\n", getprocn(1));
    printf("Runnable: %d\n", getprocn(2));
    printf("Sleeping: %d\n", getprocn(3));

    exit(0);
  }

  // normal
  if(state == 0 || state == 1)
    printf("Running:  %d\n", getprocn(1));
  if(state == 0 || state == 2)
    printf("Runnable: %d\n", getprocn(2));
  if(state == 0 || state == 3)
    printf("Sleeping: %d\n", getprocn(3));

  exit(0);
}
