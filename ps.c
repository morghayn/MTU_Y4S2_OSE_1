#include "types.h"
#include "stat.h"
#include "user.h"
#include "uproc.h"
#include "param.h"

enum procstate
{
    UNUSED,
    EMBRYO,
    SLEEPING,
    RUNNABLE,
    RUNNING,
    ZOMBIE
};

int main(int argc, char *argv[])
{
    static char *states[] = {
        [UNUSED] "unused",
        [EMBRYO] "embryo",
        [SLEEPING] "sleep ",
        [RUNNABLE] "runble",
        [RUNNING] "run   ",
        [ZOMBIE] "zombie",
    };

    struct uproc ptable[NPROC];
    struct uproc *p;
    int res;

    res = getprocs(NPROC, &ptable);
    if (res == -1)
    {
        printf(1, "An error occured running ps command.");
    }
    else
    {
        printf(1, "Number of processes in existence: %d\n", res);
        p = &ptable[0];
        printf(1, "PID, PPID, STATE, MEM (B), CMD \n");
        while (p != &ptable[NPROC - 1] && p->state != 0)
        {
            printf(1, "%d | %d | %s | %dB | %s \n", p->pid, p->ppid, states[p->state], p->sz, p->name);
            p++;
        }
    }

    exit();
}