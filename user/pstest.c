#include "kernel/types.h"
#include "user/user.h"

#define NBUSY 4
#define NSLEEP 2

int
main(int argc, char *argv[])
{
  int state, test_mode, i, pid;
  int busy_pid[NBUSY];
  int nchild;

  state = 0;
  test_mode = 0;

  if(argc >= 2){
    if(strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "-t") == 0)
      test_mode = 1;
    else
      state = atoi(argv[1]);
  }

  if(test_mode){
    nchild = NSLEEP + NBUSY;

    // 1. basline
    printf("=== basline ===\n");
    printf("Running:  %d\n", getprocn(1));
    printf("Runnable: %d\n", getprocn(2));
    printf("Sleeping: %d\n", getprocn(3));

    // 2. create SLEEPING children
    for(i = 0; i < NSLEEP; i++){
      if(fork() == 0){
        sleep(50);
        exit(0);
      }
    }

    // 3. create busy-loop children (will be RUNNABLE or RUNNING)
    for(i = 0; i < NBUSY; i++){
      pid = fork();
      if(pid == 0){
        while(1);  // busy-loop, parent will kill it
      }
      busy_pid[i] = pid;
    }

    // 4. let states settle then snapshot
    sleep(5);
    printf("=== with test processes ===\n");
    printf("Running:  %d\n", getprocn(1));
    printf("Runnable: %d\n", getprocn(2));
    printf("Sleeping: %d\n", getprocn(3));

    // 5. kill busy-loop children
    for(i = 0; i < NBUSY; i++)
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

  // normal mode: query states
  if(state == 0 || state == 1)
    printf("Running:  %d\n", getprocn(1));
  if(state == 0 || state == 2)
    printf("Runnable: %d\n", getprocn(2));
  if(state == 0 || state == 3)
    printf("Sleeping: %d\n", getprocn(3));

  exit(0);
}
