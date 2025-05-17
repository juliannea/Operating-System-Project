//Julianne Aguilar 
#include "SimOS.h"

  //Parametized Constructor
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
  CPU_ = Process(pid_, priority_, sizeOfOS_, -1);


 }


  bool SimOS::NewProcess(unsigned long long size, int priority){
    //first check if can fit in RAM memory
    if(!RAM_.canAdd(size)){
      return false;
    }
    pidTracker_ += 1; //update pid 
    int newPID = pidTracker_;
    
    //create memory item for process
    MemoryItem newItem{
      0, //default addr, real addr is determined when added 
      size,
      newPID
    };
    //add process memory to RAM 
    RAM_.addToMemory(newItem);
    
    //create new process 
    Process newProcess(newPID, priority, size);

    //determine if adding to ready queue or CPU 
    processPlacement(newProcess);
    return true;
  }

  bool SimOS::SimFork()
  {
    //check that its not OS and can fit in memory 
    if(CPU_.getPID() <= 1 || !RAM_.canAdd(CPU_.getSize())){
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
    Process childProcess(childPID, CPU_.getPriority(), CPU_.getSize(), CPU_.getPID());
    readyQueue.push_back(childProcess);
    sort();

    

    return true;
  }

  void SimOS::SimExit(){
    if(CPU_.getPID() > 1){ //check not the OS process 
      Process temp = CPU_; 

      terminate(CPU_); //first terminates all it's children, then terminates process in CPU,

      
      yieldCPU(); //exit process after releasing all its resources,  
      
      //determine if becomes zombie process 
      int parentPID = temp.getParentPID(); //get the parent PID

      if(parentPID > 1){//means has a parent, so check if parent is waiting else turn to zombie
        bool parentWaiting = false;
        int waitingIndex;

        //check if parent is waiting, in the waiting queue 
        for(int i = 0; i < waitingProcesses.size(); i++){
          if(waitingProcesses[i].getPID() == parentPID){
            parentWaiting = true;
            waitingIndex = i;
          }
        }

        if(parentWaiting){
          //current process terminates and parent "goes" to ready queue or CPU 
          processPlacement(waitingProcesses[waitingIndex]);
          //remove parent from waiting queue 
          waitingProcesses.erase(waitingProcesses.begin() + waitingIndex);
        }
        else{ //terminates and becomes a zombie process
          zombieProcesses.push_back(temp);
        }
      } 
         
    }
  }

  void SimOS::SimWait(){
    if(CPU_.getPID() > 1){ //check not OS process 
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
        //std::cout << "HAS ZOMBIE\n" << "\n";
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
      
      inputOutputQueue.push_back({CPU_, diskNumber});
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
          if(inputOutputQueue[i].first.getPID() == pid){
          
            processPlacement(inputOutputQueue[i].first);

            inputOutputQueue.erase(inputOutputQueue.begin() + i);
            return;
          }
        } 
      }
    }

  }
  
  int SimOS::GetCPU(){
    return CPU_.getPID();
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

  void SimOS::setReadyQueuePIDs(){
    readyQueuePIDs.clear();
    for(const auto& process : readyQueue){
      readyQueuePIDs.push_back(process.getPID());
    }
    //add OS process if not in the CPU 
    if(CPU_.getPID() > 1){
      readyQueuePIDs.push_back(1); //OS process 
    }
  }

  void SimOS::sort(){
    for(int i = 1; i < readyQueue.size(); i++){
      int j = i - 1;
      while (j >= 0 and readyQueue[j + 1].getPriority() > readyQueue[j].getPriority()){
        Process temp;
        temp.setPID(readyQueue[j+1].getPID());
        temp.setPriority(readyQueue[j+1].getPriority());
        temp.setSize(readyQueue[j + 1].getSize());
        temp.setParentPID(readyQueue[j+1].getParentPID());
        
        readyQueue[j + 1] = readyQueue[j];
        readyQueue[j] = temp;
        j -= 1;
      
      }

    }
  }
  
  void SimOS::yieldCPU(){
    if(readyQueue.empty()){
      CPU_ = Process(pid_, priority_, sizeOfOS_, -1); //set CPU to OS process if ready queue is empty
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

  void SimOS::terminate(Process childProcess){
  
    //check if has child process in ready q
    for(int i = readyQueue.size() - 1; i >= 0; i--){
      //check if child process has it's own child process and terminate that as well
      if(readyQueue[i].getParentPID() == childProcess.getPID()){ //found a descendant
        terminate(readyQueue[i]);
      }
    } 

    //check if any children in I/O q
    for(int i  = inputOutputQueue.size() -1; i >= 0; i--){
      //check if parent has child in I/O q, we can only call exist when parent is out of I/O q
      if(inputOutputQueue[i].first.getParentPID() == childProcess.getPID()){
        hardDisk_.terminate(inputOutputQueue[i].first.getPID(), inputOutputQueue[i].second); // PID of process, and disk number it's in
        terminate(inputOutputQueue[i].first);
        
        inputOutputQueue.erase(inputOutputQueue.begin() + i);
      }
    }
  
    //check if any children in waiting q
    for(int i = waitingProcesses.size() - 1; i >= 0; i--){
      if(waitingProcesses[i].getParentPID() == childProcess.getPID()){
        terminate(waitingProcesses[i]);

        waitingProcesses.erase(waitingProcesses.begin() + i);
      }
    }
    
    //remove child process from memory
    RAM_.removeMemoryItem(childProcess.getPID());
    //remove from ready queue if it's a child and in the ready queue
    int index = -1; //index to erase at
    for(int i = readyQueue.size() -1; i >= 0; i--){
      if(readyQueue[i].getPID() == childProcess.getPID()){
        index = i;
      }
    }
    if (index >=0){
      readyQueue.erase(readyQueue.begin() + index);
    }
  }

  int SimOS::getNumDisk()
  {
    return numberOfDisks_;
  }

  //getters
  int SimOS::getProcessRunningPriority() const{
    return CPU_.getPriority();
  }

  void SimOS::setProcessRunning(Process processRunning){
    CPU_ = processRunning;
  }

  //Displays for testing
  void SimOS::displayMemoryBlocks(){
    RAM_.displayMemoryBlocks();
  }

  void SimOS::displayRunningProcess() const
  {
    std::cout<<"Running Process: \n";
    std::cout << "PID: " << CPU_.getPID();
    std::cout << " Priority: " << CPU_.getPriority();
    std::cout << " Size: " << CPU_.getSize();
    std::cout << " Parent PID: " << CPU_.getParentPID() << "\n \n";
    std::cout <<"Children if have any: \n";
    
    for (int i = 0; i < readyQueue.size(); i++) {
      if(readyQueue[i].getParentPID() == CPU_.getPID()){
        std::cout << " PID: " <<  readyQueue[i].getPID()
                << " priority: " << readyQueue[i].getPriority()
                << " parent PID: "<< readyQueue[i].getParentPID() <<std::endl;
      }
    }
    std::cout << "\n\n";
  }

  void SimOS::displayReadyQueue() const{
    std::cout << "Displaying Ready Queue\n";
    int i = 0;
    for (const auto& process : readyQueue) {
      std::cout << "Index: " << i << ", PID: " <<  process.getPID() << std::dec
                << " priority: " << process.getPriority()
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
                <<  " parent PID: " << process.getParentPID() << std::endl;
      i++;
    }
    std::cout << "\n\n";
  }
  
  void SimOS::displayInputOutput() const{
    std::cout << "Displaying I/O Queue\n";
    int i = 0;
    for (const auto& process : inputOutputQueue) {
      std::cout << "Index: " << i << ", PID: " <<  process.first.getPID() << std::dec
                << " priority: " << process.first.getPriority()
                <<  " parent PID: " << process.first.getParentPID() 
                <<" disk reading: " << process.second << std::endl;
      i++;
    }
    std::cout << "\n\n";
  }
 
  void SimOS::displayHardDisk() const{
    hardDisk_.displayDisks();
    std::cout << "\n\n";
  }