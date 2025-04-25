//Julianne Aguilar 
#include "SimOS.h"

  /**
  Parameterized Constructor. 
  The parameters specify the number of hard disks in the simulated computer and amount of memory.
  Disk enumeration starts from 0.
  @param: sizeOfOS specifies the size of the OS process. It has the PID of 1, priority of 0, and resides in the very beginning of memory.
  */
 SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned long long sizeOfOS){
  numberOfDisks_ = numberOfDisks;
  sizeOfOS_ = sizeOfOS;
  RAM_.setMemAmount(amountOfRAM);

  //now we have to add the OS process to RAM b/c it take memory space and has an address in the begining of memory


 }

  /**
  Creates a new process with the specified priority in the simulated system. The new process takes place in the ready-queue or immediately starts using the CPU.
  Every process in the simulated system has a PID. Your simulation assigns PIDs to new processes starting from 2 (1 is reserved for the OS) and increments it by one for each new process. Do not reuse PIDs of the terminated processes.
  For example, the command NewProcess(1000, 5) means that a new process with priority level 5 should be created and it requires 1000 bytes of memory.
  NewProcess returns true if a new process was successfully created and false if otherwise. One of the reasons a process wasn’t created is insufficient free memory in the system.
  */
  bool SimOS::NewProcess(unsigned long long size, int priority){
   
  }

