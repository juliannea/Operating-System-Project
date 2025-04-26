//Julianne Aguilar 

#include "Process.h"


  /**
  Creates new process 
  @param pid: PID of the new process 
  @param priority: priority of the new process
  @param sizes: the size of the process 
  @param address: the memory address (it's location in memory)
  */
Process::Process(int pid, int priority, unsigned long long size , unsigned long long address, int parentPID){
  pid_ = pid;
  priority_ = priority;
  size_ = size;
  address_ = address;
  parentPID_ = parentPID;
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

void Process::setParentPID(int parentPID){
  parentPID_ = parentPID;
}

void Process::addChild(Process childProcess){
  children.push_back(childProcess);
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

unsigned long long Process::getAddress() const
{
  return address_;
}

int Process::getParentPID() const{
  return parentPID_;
}

std::vector<Process> Process::getChildPIDs() const
{
  return children;
}
