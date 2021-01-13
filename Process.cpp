#include "Process.h"

Process::Process(int pid) : status(0) {
    this->pid = pid;
}

void Process::switchstatus() {
    this->status = (this->status == true ? false : true);
}

