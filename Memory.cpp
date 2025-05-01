//Julianne Aguilar 

#include "Memory.h"
//Default Constructor
Memory::Memory(){

}

//Parametized Constructor
Memory::Memory(unsigned long long amountOfRam){
  amountOfRam_ = amountOfRam;
   //add the memory amount to the memoryblock vector
   MemoryItem newItem{
    0, //address
    amountOfRam, //size of start block (nothing in it yet)
    NO_PROCESS //no process 
  };
  memoryBlocks_.push_back(newItem);
}

//Checks if can add process to memeory 
bool Memory::canAdd(unsigned long long processSize){
  for(const auto& block: memoryBlocks_){
    if(processSize <= block.itemSize && block.PID == -1){
      return true;
    }
  }
  return false;
}

//Removes specified process from RAM memory 
void Memory::removeMemoryItem(int pid){
  //find index of memory item with the PID 
  int freeIndex;
  for(int i = 0; i < memoryBlocks_.size(); i++){
    if(memoryBlocks_[i].PID == pid){
      freeIndex = i;
    }
  }

  memoryBlocks_[freeIndex].PID = NO_PROCESS; //means free 
 
 
  //check if the one before is a free memory, if so combine the mem hole 
  if(memoryBlocks_[freeIndex-1].PID ==  NO_PROCESS && memoryBlocks_[freeIndex + 1].PID == NO_PROCESS){
    memoryBlocks_[freeIndex - 1].itemSize += memoryBlocks_[freeIndex].itemSize;
    memoryBlocks_.erase(memoryBlocks_.begin() + freeIndex); 
     
    memoryBlocks_[freeIndex - 1].itemSize += memoryBlocks_[freeIndex].itemSize;
    memoryBlocks_.erase(memoryBlocks_.begin() + freeIndex); 
  }
  else if(memoryBlocks_[freeIndex-1].PID ==  NO_PROCESS){ //means no process, free memory
   
    memoryBlocks_[freeIndex - 1].itemSize += memoryBlocks_[freeIndex].itemSize;
    memoryBlocks_.erase(memoryBlocks_.begin() + freeIndex); 
  }
  else if(memoryBlocks_[freeIndex + 1].PID == NO_PROCESS){
  
    memoryBlocks_[freeIndex + 1].itemSize += memoryBlocks_[freeIndex].itemSize;
    memoryBlocks_.erase(memoryBlocks_.begin() + freeIndex); 
  }
  
}

//Adds process's memory block to RAM to the largest hole - worst fit approach
bool Memory::addToMemory(MemoryItem memoryBlock){
  //if memoryBlocks vector empyty mean initalize it with the starting RAM memory 
  if(memoryBlocks_.empty()){
    memoryBlocks_.push_back(memoryBlock);
    return true;
  }
  //if not empty means we have to iterate through and find the largest block
  else{
    int currMax = 0;
    int maxIndex; 

    for(int i = 0; i < memoryBlocks_.size();i++){
      if(memoryBlocks_[i].itemSize > currMax && memoryBlocks_[i].PID == NO_PROCESS){
        currMax = memoryBlocks_[i].itemSize;
        maxIndex = i;
      }
    }
    
    //check if can fit memory block in the largest free memory, then add it 
    if(memoryBlock.itemSize <= memoryBlocks_[maxIndex].itemSize){
      memoryBlock.itemAddress = maxIndex; 
      memoryBlocks_.insert(memoryBlocks_.begin() + maxIndex, memoryBlock);
      memoryBlocks_[maxIndex + 1].itemSize = memoryBlocks_[maxIndex + 1].itemSize - memoryBlock.itemSize; //update size of free memory
      memoryBlocks_[maxIndex + 1].itemAddress = maxIndex + 1; //update free memory position in memory
 
      return true;
    }
   
  }
  return false;
}

//sets the processes in memory
void Memory::setProcessesInMem(){
  processesInMem.clear();
  for (const auto& block : memoryBlocks_){
    if(block.PID != NO_PROCESS){
      processesInMem.push_back(block);

    }
  }
}

void Memory::setMemAmount(unsigned long long amountOfRam){
  amountOfRam_ = amountOfRam;
} // return the processes in memory in order from lowest to highest address

MemoryUse Memory::getProcessesInMem()
{
  setProcessesInMem();
  return processesInMem;
}

// Displays for testing 
void Memory::displayMemoryBlocks(){
  std::cout <<"Displaying Memory Blocks: \n";
  int i = 0;
  for (const auto& block : memoryBlocks_) {
    std::cout << "index: " << i << ", Address: 0x" << std::hex << block.itemAddress << std::dec
              << ", Size: " << block.itemSize
              << ", PID: " << block.PID << "\n";
    i++;
  }
  std::cout << "\n\n";
}