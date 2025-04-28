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
  MemoryItem newItem{
    0, //address
    amountOfRAM, //size of start block (nothing in it yet)
    NO_PROCESS //no process 
  };
  RAM_.addToMemory(newItem);

  //now we have to add the OS process to RAM b/c it take memory space and has an address in the begining of memory
  MemoryItem OS{
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
    MemoryItem newItem{
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
    sort();
    return true;
  }

  bool SimOS::SimFork()
  {
    //check that its not OS and can fit in memory 
    if(processRunning_.getPID() == 1 || !RAM_.canAdd(processRunning_.getSize())){
      return false; 
    }
    //create child process: priority & size inherited from parent, PID different place in memory different
    pidTracker_ += 1;
    int childPID = pidTracker_;

    //create memory block for child process 
    MemoryItem childMem{
      0, //default address, real addr is set when added 
      processRunning_.getSize(),
      childPID
    };
    RAM_.addToMemory(childMem);

    //create child process 
    Process childProcess(childPID, processRunning_.getPriority(), processRunning_.getSize(), RAM_.getAddress(childPID), processRunning_.getPID());
    readyQueue.push_back(childProcess);
    sort();

    //add child process to parents childPIDs queue
    processRunning_.addChild(childProcess);

    return true;
  }

  void SimOS::SimExit(){
    //check if the parent is waiting or if it has no parent, no parent means no worries about zombie 
    if(processRunning_.getPID() != 1){
      int parentPID = processRunning_.getParentPID();
      bool parentWaiting = false;
      int waitingIndex;

      for(int i = 0; i < waitingProcesses.size(); i++){
        if(waitingProcesses[i].getPID() == parentPID){
          parentWaiting = true;
          waitingIndex = i;
        }
      }

      //if parent is waiting, current process terminates and parent "goes" to ready queue or CPU 
      if(parentWaiting){
        //move parent to ready queue
        readyQueue.push_back(waitingProcesses[waitingIndex]);
        sort(); //sort ready queue 

        //remove parent process from waitingProcess queue 
        waitingProcesses.erase(waitingProcesses.begin() + waitingIndex);

      }
      //case turns into zombie process
      else if(!parentWaiting && processRunning_.getParentPID() != -1){
        zombieProcesses.push_back(processRunning_);
      }

       //remove process from memory only if not a zombie process 
      if(parentWaiting || processRunning_.getParentPID() == -1){
        RAM_.removeMemoryItem(processRunning_.getPID());

        //terminate all its descendants as well, remove all its children from memory and the ready queue
        std::vector<Process> parentsChildren = processRunning_.getChildren();
        for(int i = 0; i < parentsChildren.size(); i++){
          //remove from memory 
          RAM_.removeMemoryItem(parentsChildren[i].getPID());
          //remove from ready queue 
          for(int j = readyQueue.size() -1; j >= 0; j--){
            if(readyQueue[j].getPID() == parentsChildren[i].getPID()){
              readyQueue.erase(readyQueue.begin() + j);
            }
          }
        }   

      }
 
      //just for testing purposes comment out later
      //readyQueue.erase(readyQueue.begin());

       //new process in CPU 
       if(!readyQueue.empty()){
        processRunning_ = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
       }
       else{
        processRunning_ = Process(0,0,0,0);
       }
       

    }
    
  }

  void SimOS::SimWait(){
    //currently running process enters waiting queue if there's no zombie child 
    //check if has zombie child, check if PID = 1 means OS, if those conditions don't pass then enters waiting queue 
    //if wait is over - child terminates, the process foes to end of ready queue or CPU - this is handled in the Exit function 

    if(processRunning_.getPID() != 1 && !processRunning_.getChildren().empty()){ //makes sure process has children
      bool hasZombie = false;
      int zombieIndex;

      for(int i = 0; i < zombieProcesses.size(); i++){
        if(zombieProcesses[i].getParentPID() == processRunning_.getPID()){
         hasZombie = true;
         zombieIndex = i;
        }
      }
          
      if(hasZombie){
        //removce zombie from memory 
        RAM_.removeMemoryItem(zombieProcesses[zombieIndex].getPID());
        //zombie child disappears 
        zombieProcesses.erase(zombieProcesses.begin() + zombieIndex);
      }
      else{
        //no zombie so process pauses(yields the CPU) and enters waiting queue 
        waitingProcesses.push_back(processRunning_);
        if(!readyQueue.empty()){
          processRunning_ = readyQueue[0];
          readyQueue.erase(readyQueue.begin());
        }
        else{
          processRunning_ = Process(0,0,0,0);
        }

      }
       
    }

  }

  int SimOS::GetCPU(){
    return processRunning_.getPID();
  }

  std::vector<int> SimOS::GetReadyQueue(){
    for(const auto& process : readyQueue){
      readyQueuePIDs.push_back(process.getPID());
    }
    return readyQueuePIDs;
  }

  MemoryUse SimOS::GetMemory()
  {
    return RAM_.getProcessesInMem();
  }

  void SimOS::sort(){
    for(int i = 1; i < readyQueue.size(); i++){
      int j = i - 1;
      while (j >= 0 and readyQueue[j + 1].getPriority() > readyQueue[j].getPriority()){
        Process temp;
        temp.setPID(readyQueue[j+1].getPID());
        temp.setPriority(readyQueue[j+1].getPriority());
        temp.setSize(readyQueue[j + 1].getSize());
        temp.setAddress(readyQueue[j+1].getAddress());
        temp.setParentPID(readyQueue[j+1].getParentPID());
        
        readyQueue[j + 1] = readyQueue[j];
        readyQueue[j] = temp;
        j -= 1;
      
      }

    }
  }
  //getters
  int SimOS::getProcessRunningPriority() const{
    return processRunning_.getPriority();
  }

  void SimOS::setProcessRunning(Process processRunning){
    processRunning_ = processRunning;
  }

  //Displays for testing
  const void SimOS::displayMemoryBlocks(){
    RAM_.displayMemoryBlocks();
  }

  void SimOS::displayRunningProcess() const
  {
    std::cout<<"Running Process: \n";
    std::cout << "PID: " << processRunning_.getPID();
    std::cout << " Priority: " << processRunning_.getPriority();
    std::cout << " Size: " << processRunning_.getSize();
    std::cout << " Address: " << processRunning_.getAddress();
    std::cout << " Parent PID: " << processRunning_.getParentPID() << "\n \n";
    std::cout <<"Children if have any: \n";
    int i = 0;
    for (const auto& process : processRunning_.getChildren()) {
      std::cout << "Index: " << i << ", PID: " << std::hex <<  process.getPID() << std::dec
                << " priority: " << process.getPriority()
                << " address: " << process.getAddress() 
                << " parent PID: "<< process.getParentPID() <<std::endl;
      i++;
    }
    std::cout << "\n\n";
  }

  void SimOS::displayReadyQueue() const{
    std::cout << "Displaying Ready Queue\n";
    int i = 0;
    for (const auto& process : readyQueue) {
      std::cout << "Index: " << i << ", PID: " << std::hex <<  process.getPID() << std::dec
                << " priority: " << process.getPriority()
                << " address " << process.getAddress() 
                <<  " parent PID: " << process.getParentPID() << std::endl;
      i++;
    }
    std::cout << "\n\n";
  }

  void SimOS::displayWaiting() const{
    std::cout << "Displaying Waiting Proccesses Queue\n";
    int i = 0;
    for (const auto& process : waitingProcesses) {
      std::cout << "Index: " << i << ", PID: " << std::hex <<  process.getPID() << std::dec
                << " priority: " << process.getPriority()
                << " address " << process.getAddress() 
                <<  " parent PID: " << process.getParentPID() << std::endl;
      i++;
    }
    std::cout << "\n\n";
  }

  void SimOS::displayZombies() const{
    std::cout << "Displaying Zombies Queue\n";
    int i = 0;
    for (const auto& process : zombieProcesses) {
      std::cout << "Index: " << i << ", PID: " << std::hex <<  process.getPID() << std::dec
                << " priority: " << process.getPriority()
                << " address " << process.getAddress() 
                <<  " parent PID: " << process.getParentPID() << std::endl;
      i++;
    }
    std::cout << "\n\n";
  }
