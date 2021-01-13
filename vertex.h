#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
#include "Resource.h"

//边的表示，start=0从进程指向边，start=1从边指向进程
class vertex {
public:
    vertex(int pid, int rid, int start);
    int pid;
    int rid;
    int start;
};