struct uproc
{
    int pid;         // process id
    int ppid;        // parent process id
    int state;       // process state
    uint sz;         // size of process memory (bytes)
    char name[16];   // process name
};