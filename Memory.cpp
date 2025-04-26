//Julianne Aguilar 

#include "Memory.h"


//Default Constructor
Memory::Memory(){

}

//Parametized Constructor
Memory::Memory(unsigned long long amountOfRam){
  amountOfRam_ = amountOfRam;
}

/**
Checks if there's enough memory space for the process*/
bool Memory::canAdd(unsigned long long processSize){
  for(const auto& block: memoryBlocks_){
    if(processSize <= block.itemSize && block.PID == -1){
      return true;
    }
  }
  return false;
}

bool Memory::addToMemory(MemoryItem memoryBlock){
  //if memoryBlocks vector empyty mean initalize it with the starting RAM memory 
  if(memoryBlocks_.empty()){
    //just push it 
    memoryBlocks_.push_back(memoryBlock);
    return true;
  }
  //if not empty means we have to iterate through and find the largest block
  else{
    int currMax = 0;
    int maxIndex; 

    for(int i = 0; i < memoryBlocks_.size();i++){
      if(memoryBlocks_[i].itemSize > currMax && memoryBlocks_[i].PID == -1){
        currMax = memoryBlocks_[i].itemSize;
        maxIndex = i;
      }
    }
    
    //check if can fit memory block in the largest free memory, then add it 
    if(memoryBlock.itemSize <= memoryBlocks_[maxIndex].itemSize){
      memoryBlock.itemAddress = maxIndex; 
      memoryBlocks_.insert(memoryBlocks_.begin() + maxIndex, memoryBlock);
      memoryBlocks_[maxIndex + 1].itemSize = memoryBlocks_[maxIndex + 1].itemSize - memoryBlock.itemSize;
      memoryBlocks_[maxIndex + 1].itemAddress = maxIndex + 1;
 
      return true;
    }
   
  }
  return false;
}

/**
Gets the memory address of the process 
@param: pid of the process you want the address of 
@return: address of the process with the pid 
*/
unsigned long long Memory::getAddress(int pid) const
{
  for(const auto& block : memoryBlocks_){
    if(pid == block.PID){
      return block.itemAddress;
    }
  }
  return 0;
}

//setters
void Memory::setMemAmount(unsigned long long amountOfRam){

  amountOfRam_ = amountOfRam;
}

// Displays for testing 
void Memory::displayMemoryBlocks(){
  int i = 0;
  for (const auto& block : memoryBlocks_) {
    std::cout << "index: " << i << ", Address: 0x" << std::hex << block.itemAddress << std::dec
              << ", Size: " << block.itemSize
              << ", PID: " << block.PID << std::endl;
    i++;
  }
}