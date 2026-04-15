#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char* argv[]) {
   int state=-1;
    if(argc == 2){
     state = atoi(argv[1]);
    }
    if(state ==1){
        if(fork() == 0){
            while(1);
        }
    }
    if(state ==2){
        if(fork() == 0){
            sleep(1000);
        }
    }
    
    // 父进程调用你的系统调用
    printf("Running: %d\n", getprocn(1));
    printf("Runnable: %d\n", getprocn(2));
    printf("Sleeping: %d\n", getprocn(3));
    exit(0);
}