//Julianne Aguilar 

#pragma once

class Process{
  public:
    Process();
    /**
    Creates new process 
      @param pid: PID of the new process 
      @param priority: priority of the new process
      @param sizes: the size of the process 
      @param address: the memory address (it's location in memory)
    */
    Process(int pid, int priority, unsigned long long size, unsigned long long address);

    void setPID(int pid);

    void setPriority(int priority);

    void setSize(unsigned long long size);

    void setAddress(unsigned long long address);

  private:
    int pid_;
    int priority_;
    unsigned long long size_;
    unsigned long long address_;

};