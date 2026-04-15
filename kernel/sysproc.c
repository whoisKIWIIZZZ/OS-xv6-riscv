#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


// 在 kernel/sysproc.c 中
extern struct proc proc[NPROC]; // 声明外部进程表

uint64
sys_getprocn(void)
{
  int n;
  int count = 0;
  
  // 1. 获取用户传入的第一个整型参数
  argint(0, &n);

  // 2. 将参数映射为内核中的进程状态常量
  enum procstate target_state;
  if(n == 1) target_state = RUNNING;
  else if(n == 2) target_state = RUNNABLE;
  else if(n == 3) target_state = SLEEPING;
  else return -1;

  // 3. 遍历进程表进行计数
  struct proc *p;
  for(p = proc; p < &proc[NPROC]; p++){
    acquire(&p->lock); // 必须加锁以保证读取状态时的一致性
    if(p->state == target_state) {
      count++;
    }
    release(&p->lock);
  }

  return count; // 返回值会自动放入 a0
}