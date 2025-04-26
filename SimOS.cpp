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

  //add the memory amount to the memoryblock vector
  Memory::MemoryItem newItem{
    0, //address
    amountOfRAM, //size of start block (nothing in it yet)
    NO_PROCESS //no process 
  };
  RAM_.addToMemory(newItem);

  //now we have to add the OS process to RAM b/c it take memory space and has an address in the begining of memory
  Memory::MemoryItem OS{
    address, //address
    sizeOfOS, 
    pid_, //PID
  };
  RAM_.addToMemory(OS);

 }

  /**
  Creates a new process with the specified priority in the simulated system. The new process takes place in the ready-queue or immediately starts using the CPU.
  Every process in the simulated system has a PID. Your simulation assigns PIDs to new processes starting from 2 (1 is reserved for the OS) and increments it by one for each new process. Do not reuse PIDs of the terminated processes.
  For example, the command NewProcess(1000, 5) means that a new process with priority level 5 should be created and it requires 1000 bytes of memory.
  NewProcess returns true if a new process was successfully created and false if otherwise. One of the reasons a process wasn’t created is insufficient free memory in the system.
  */
  bool SimOS::NewProcess(unsigned long long size, int priority){
    //first check if can fit in RAM memory
    if(!RAM_.canAdd(size)){
      return false;
    }

    pidTracker_ += 1; //update pid 
    int newPID = pidTracker_;
    
    //create memory item 
    Memory::MemoryItem newItem{
      0, //default addr, addr is determined when added 
      size,
      newPID
    };
    //add to RAM 
    RAM_.addToMemory(newItem);
    
    //create new process 
    Process newProcess(newPID, priority, size, RAM_.getAddress(newPID));

    //determine if adding to ready queue or CPU 
    if(priority > processRunning_.getPriority()){
      //add the current process that's running to the ready queue if its not currently a default one or the OS
      if(processRunning_.getPID() > 1){
        readyQueue.push_back(processRunning_);
      }
      processRunning_ = newProcess;
    }
    else{
      readyQueue.push_back(newProcess);
    }
    return true;
  }

  bool SimOS::SimFork()
  {
    //check that its not OS
    if(processRunning_.getPID() == 1){
      return false; 
    }
    bool ans =  NewProcess(processRunning_.getSize(), processRunning_.getPriority());
  }

  //Displays for testing
  const void SimOS::displayMemoryBlocks(){
    RAM_.displayMemoryBlocks();
  }

  void SimOS::displayRunningProcess() const
  {
    std::cout<<"Running Process: \n" << "PID: " << processRunning_.getPID() << "\nsize: " << processRunning_.getSize() << "\naddress: " << processRunning_.getAddress() << "\npriority: " << processRunning_.getPriority() << "\n";
  }

  void SimOS::displayReadyQueue() const{
    int i = 0;
    for (const auto& process : readyQueue) {
      std::cout << "Index: " << i << ", PID: " << std::hex <<  process.getPID() << std::dec
                << " priority: " << process.getPriority()
                << " address " << process.getAddress() << std::endl;
      i++;
    }
  }
