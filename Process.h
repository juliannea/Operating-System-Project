//Julianne Aguilar 

#pragma once
#include <vector>
class Process{
  public:
    /**
    Creates new process 
      @param pid: PID of the new process 
      @param priority: priority of the new process
      @param sizes: the size of the process 
      @param address: the memory address (it's location in memory)
      @param parentPID: PID of the parent process if it's a child process, set to -1 if no parent process
    */
    Process(int pid = -1, int priority = -1, unsigned long long size = -1, unsigned long long address = -1, int parentPID = -1);

    //setters
    void setPID(int pid);

    void setPriority(int priority);

    void setSize(unsigned long long size);

    void setAddress(unsigned long long address);

    void setParentPID(int parentPID);

    void addChild(Process childProcess);

    //getters
    int getPID() const;

    int getPriority() const;

    unsigned long long getSize() const;

    unsigned long long getAddress() const;

  private:
    int pid_;
    int priority_;
    unsigned long long size_;
    unsigned long long address_;
    int parentPID_ = -1;
    std::vector<Process> childPIDs;
};