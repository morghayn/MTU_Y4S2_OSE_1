#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "trace.h"
#include "uproc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_trace(void)
{
  int n;
  argint(0, &n);
  struct proc *curproc = myproc();
  curproc->traced = n;
  return 0;
}

// pointer for ptable array and pointer for ptable end address
extern struct proc *getproc_ptable_addr(void);
extern struct proc *getproc_ptable_end_addr(void);

int sys_getprocs(void)
{
  int size;
  struct uproc *p;
  int i = 0;
  // int debug = 0;

  // if argint has trouble, return error -1
  if (argint(0, &size) < 0)
  {
    return -1;
  }
  if (argptr(1, (void *)&p, size * sizeof(struct uproc)) < 0)
  {
    return -1;
  }

  // get pointer to ptable and end address of ptable array
  struct proc *ptr = getproc_ptable_addr();
  struct proc *ptr_end = getproc_ptable_end_addr();
  
  // increment ptr array until out of bounds
  for (; ptr < ptr_end; ptr++)
  {
    // for debugging, loop will iterate 64 times ass per NPROC (max processes supported)
    // debug++;
    // cprintf("%d. %x < %x\n", debug, ptr, ptr_end);

    if (!(ptr->state == UNUSED))
    {
      // copying pid, ppid, process name, state, sz bytes from ptable if not UNUSED
      p[i].pid = ptr->pid;
      p[i].ppid = ptr->parent ? ptr->parent->pid : 0; // init process does not have a parent process
      strncpy(p[i].name, ptr->name, 16);
      p[i].state = ptr->state;
      p[i].sz = ptr->sz;
      i++;
    }
  }

  // // returns number of processes in existence,
  // // else if error we would've returned prior to now and -1 would have been returned
  return i;
}