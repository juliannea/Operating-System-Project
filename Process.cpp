//Julianne Aguilar 

#include "Process.h"

  //Default Constructor 
  Process::Process(){

  }
  /**
  Creates new process 
  @param pid: PID of the new process 
  @param priority: priority of the new process
  @param sizes: the size of the process 
  @param address: the memory address (it's location in memory)
  */
Process::Process(int pid, int priority, unsigned long long size, unsigned long long address){
  pid_ = pid;
  priority_ = priority;
  size_ = size;
  address_ = address;
}

//setters
void Process::setPID(int pid){
  pid_ = pid;
}

void Process::setPriority(int priority){
  priority_ = priority;
}

void Process::setSize(unsigned long long size){
  size_ = size;
}

void Process::setAddress(unsigned long long address){
  address_ = address;
}