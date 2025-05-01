//Julianne Aguilar 

#include "Process.h"


  /**
  Creates new process 
  @param pid: PID of the new process 
  @param priority: priority of the new process
  @param sizes: the size of the process 
  @param address: the memory address (it's location in memory)
  */
Process::Process(int pid, int priority, unsigned long long size, int parentPID){
  pid_ = pid;
  priority_ = priority;
  size_ = size;
  parentPID_ = parentPID;

}

void Process::addChild(Process childProcess){
  children.push_back(childProcess);
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


void Process::setParentPID(int parentPID){
  parentPID_ = parentPID;
}

//getters
int Process::getPID() const
{
  return pid_;
}

int Process::getPriority() const
{
  return priority_;
}

unsigned long long Process::getSize() const
{
  return size_;
}


int Process::getParentPID() const{
  return parentPID_;
}

std::vector<Process> Process::getChildren() const
{
  return children;
}

