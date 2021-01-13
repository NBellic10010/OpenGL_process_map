#include <stdio.h>
#include <stdlib.h>

//进程的结构表示
class Process {
public:
    Process(int pid);
    int getpid(){ return pid; }
    bool getstatus(){ return status; }
    void switchstatus();
    int x, y;
private:
    int pid;
    bool status;
};