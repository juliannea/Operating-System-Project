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
  hardDisk_ = Disk(numberOfDisks_);
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
  //create OS process and set it to the CPU_ 
  CPU_ = Process(pid_, priority_, sizeOfOS_, 0, -1);


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
    processPlacement(newProcess);
    return true;
  }

  bool SimOS::SimFork()
  {
    //check that its not OS and can fit in memory 
    if(CPU_.getPID() == 1 || !RAM_.canAdd(CPU_.getSize())){
      return false; 
    }
    //create child process: priority & size inherited from parent, PID different place in memory different
    pidTracker_ += 1;
    int childPID = pidTracker_;

    //create memory block for child process 
    MemoryItem childMem{
      0, //default address, real addr is set when added 
      CPU_.getSize(),
      childPID
    };
    RAM_.addToMemory(childMem);

    //create child process 
    Process childProcess(childPID, CPU_.getPriority(), CPU_.getSize(), RAM_.getAddress(childPID), CPU_.getPID());
    readyQueue.push_back(childProcess);
    sort();

    //add child process to parents childPIDs queue
    CPU_.addChild(childProcess);

    return true;
  }

  void SimOS::SimExit(){
    if(CPU_.getPID() > 1){ //check not the OS process 
      Process temp = CPU_; 
      RAM_.removeMemoryItem(temp.getPID()); //release memory used by this process immediately 

      //terminate all its descendants as well, remove all its children from memory and the ready queue
      std::vector<Process> parentsChildren = temp.getChildren();
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
            
      yieldCPU(); //exit process currently using CPU 
      


      int parentPID = temp.getParentPID(); //get the parent PID
      bool parentWaiting = false;
      int waitingIndex;

      //check if parent is waiting, in the waiting queue 
      for(int i = 0; i < waitingProcesses.size(); i++){
        if(waitingProcesses[i].getPID() == parentPID){
          parentWaiting = true;
          waitingIndex = i;
        }
      }

      //if parent is waiting, current process terminates and parent "goes" to ready queue or CPU 
      if(parentWaiting){
        //move parent to ready queue or CPU
        processPlacement(waitingProcesses[waitingIndex]);
        //remove parent process from waitingProcess queue 
        waitingProcesses.erase(waitingProcesses.begin() + waitingIndex);
      }
      //if parent hasn't called wait yet, and has a parent, process turns into zombie
      else if(!parentWaiting && temp.getParentPID() != -1){
        zombieProcesses.push_back(temp);
      }    

    }
  }

  void SimOS::SimWait(){
    if(CPU_.getPID() > 1 && !CPU_.getChildren().empty()){ //check not OS process and if it has children - no point in waiting if not children
      bool hasZombie = false;
      int zombieIndex;

      //check if has zombie process
      for(int i = 0; i < zombieProcesses.size(); i++){
        if(zombieProcesses[i].getParentPID() == CPU_.getPID()){
         hasZombie = true;
         zombieIndex = i;
        }
      }
          
      if(hasZombie){
        //zombie child disappears 
        zombieProcesses.erase(zombieProcesses.begin() + zombieIndex);
        //process continues running
      }
      else{
        //no zombie so process pauses(yields the CPU) and enters waiting queue 
        waitingProcesses.push_back(CPU_);
        yieldCPU();

      }
    }

  }

  void SimOS::DiskReadRequest( int diskNumber, std::string fileName ){
    if(CPU_.getPID() > 1 && hardDisk_.diskExist(diskNumber)){
      //currently running process requests to read the specified file from the disk with given number
      hardDisk_.readRequest(diskNumber, fileName, CPU_.getPID());
      //the process issuing the disk read request stops using (yields) the CPU, 
      //first move the process to the I/O queue 
      inputOutputQueue.push_back(CPU_);
      yieldCPU();
    }

  }

  void SimOS::DiskJobCompleted( int diskNumber){
    if(hardDisk_.diskExist(diskNumber)){

      int pid = hardDisk_.completeRequest(diskNumber); //pid of the process whose request was completed
      if(pid > 1){
        //the served process should return to the ready-queue or start using the CPU 
        //find the process in the I/O queue 
        for(int i = 0; i < inputOutputQueue.size(); i++){
          if(inputOutputQueue[i].getPID() == pid){
          
            processPlacement(inputOutputQueue[i]);

            inputOutputQueue.erase(inputOutputQueue.begin() + i);
            sort();
            return;
          }
        } 
      }
    }

  }
  
  int SimOS::GetCPU(){
    return CPU_.getPID();
  }

  void SimOS::setReadyQueuePIDs(){
    readyQueuePIDs.clear();
    for(const auto& process : readyQueue){
      readyQueuePIDs.push_back(process.getPID());
    }
  }
  
  std::vector<int> SimOS::GetReadyQueue(){
    setReadyQueuePIDs();
    return readyQueuePIDs;
  }

  MemoryUse SimOS::GetMemory()
  {
    return RAM_.getProcessesInMem();
  }

  FileReadRequest SimOS::GetDisk( int diskNumber){
    return hardDisk_.getDiskRequest(diskNumber);
  }
  
  std::queue<FileReadRequest> SimOS::GetDiskQueue(int diskNumber){
    return hardDisk_.getDiskQueue(diskNumber);
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
  
  void SimOS::yieldCPU(){
    if(readyQueue.empty()){
      CPU_ = Process(pid_, priority_, sizeOfOS_, address, -1); //set CPU to OS process if ready queue is empty
    }
    else{
      CPU_ = readyQueue[0];
      readyQueue.erase(readyQueue.begin());
    }
  }
  
  void SimOS::processPlacement(Process newProcess){
    //move the process to CPU if priority is greater than the currently running one, make sure not OS process
    if(newProcess.getPriority() > CPU_.getPriority()){
      if(CPU_.getPID() > 1){
        readyQueue.push_back(CPU_);
      }
      
      CPU_ = newProcess;
    }
    else{
      readyQueue.push_back(newProcess);
    }
    sort();
  }

  //getters
  int SimOS::getProcessRunningPriority() const{
    return CPU_.getPriority();
  }

  void SimOS::setProcessRunning(Process processRunning){
    CPU_ = processRunning;
  }

  //Displays for testing
  const void SimOS::displayMemoryBlocks(){
    RAM_.displayMemoryBlocks();
  }

  void SimOS::displayRunningProcess() const
  {
    std::cout<<"Running Process: \n";
    std::cout << "PID: " << CPU_.getPID();
    std::cout << " Priority: " << CPU_.getPriority();
    std::cout << " Size: " << CPU_.getSize();
    std::cout << " Address: " << CPU_.getAddress();
    std::cout << " Parent PID: " << CPU_.getParentPID() << "\n \n";
    std::cout <<"Children if have any: \n";
    int i = 0;
    for (const auto& process : CPU_.getChildren()) {
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
                << " size: " << process.getSize()
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
  
  void SimOS::displayInputOutput() const{
    std::cout << "Displaying I/O Queue\n";
    int i = 0;
    for (const auto& process : inputOutputQueue) {
      std::cout << "Index: " << i << ", PID: " << std::hex <<  process.getPID() << std::dec
                << " priority: " << process.getPriority()
                << " address " << process.getAddress() 
                <<  " parent PID: " << process.getParentPID() << std::endl;
      i++;
    }
    std::cout << "\n\n";
  }
 
  void SimOS::displayHardDisk() const{
    hardDisk_.displayDisks();
  }