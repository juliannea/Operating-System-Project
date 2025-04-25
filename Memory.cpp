//Julianne Aguilar 

#include "Memory.h"

Memory::Memory(){

}

Memory::Memory(unsigned long long amountOfRam){
  amountOfRam_ = amountOfRam;
}

void Memory::setMemAmount(unsigned long long amountOfRam){
  amountOfRam_ = amountOfRam;
}

void Memory::addToMemory(MemoryItem memoryBlock){
  //if memoryBlocks vector empyty mean initalize it with the starting RAM memory 
  if(memoryBlocks_.empty()){
    //just push it 
    memoryBlocks_.push_back(memoryBlock);
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
    //should have max block now 
    //so add to vector at that maxIndex, decrease size of the maxblock by the memoryBlock we're inserting 
    //make memoryBlock adress the index it's at 
    memoryBlock.itemAddress = maxIndex; 
    memoryBlocks_.insert(memoryBlocks_.begin() + maxIndex, memoryBlock);
    memoryBlocks_[maxIndex + 1].itemSize = memoryBlocks_[maxIndex + 1].itemSize - memoryBlock.itemSize;
    memoryBlocks_[maxIndex + 1].itemAddress = maxIndex + 1;

  }
}


void Memory::getMemoryBlocks(){
  int i = 0;
  for (const auto& block : memoryBlocks_) {
    std::cout << "index: " << i << ", Address: 0x" << std::hex << block.itemAddress << std::dec
              << ", Size: " << block.itemSize
              << ", PID: " << block.PID << std::endl;
    i++;
  }
}