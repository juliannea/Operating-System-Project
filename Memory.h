//Julianne Aguilar 
#include <vector>
#pragma once
class Memory{
  public:
    Memory();

    Memory(unsigned long long amountOfRam);

    void setMemAmount(unsigned long long amountOfRam);

    struct MemoryItem{
	    unsigned long long itemAddress;
	    unsigned long long itemSize;
	    int PID; // PID of the process using this chunk of memory
    };
    
    using MemoryUse = std::vector<MemoryItem>; //processes appear in the same order they appear in memory (low address to high)
 
    





  private:
    unsigned long long amountOfRam_;
    std::vector<MemoryItem> memoryBlocks_;

};
