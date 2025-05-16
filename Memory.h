//Julianne Aguilar 
#pragma once
#include <iostream>
#include <vector>

constexpr int NO_PROCESS{-1}; //used to mark memory block free

struct MemoryItem{
  unsigned long long itemAddress;
  unsigned long long itemSize;
  int PID; // PID of the process using this chunk of memory
};

using MemoryUse = std::vector<MemoryItem>; //processes appear in the same order they appear in memory (low address to high)


class Memory{
  public:
    Memory();

    Memory(unsigned long long amountOfRam);

    
    /**
    Checks if there's enough memory space for the process
     */
    bool canAdd(unsigned long long processSize);

    /** 
    Removes the Memory Item with the PID given from memory
    */
    void removeMemoryItem(int pid);
    /**
    Adds to RAM memory if there's free memory 
    */
    bool addToMemory(MemoryItem memoryBlock);

    //setters
    void setProcessesInMem();

    void setMemAmount(unsigned long long amountOfRam);

    //getters
    MemoryUse getProcessesInMem();
    
    //Displays for testing
    void displayMemoryBlocks();

  private:
    unsigned long long amountOfRam_;
    MemoryUse memoryBlocks_;

    MemoryUse processesInMem;

};
