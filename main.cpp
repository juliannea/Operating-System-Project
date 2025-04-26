#include <iostream>
#include "SimOS.h"
#include "Memory.h"
#include "Process.h"
#include "Disk.h"

int main(){
  std::cout <<"SimOS Constructor test\n";
  SimOS testOS(1, 2048, 1024); //disknumber, amount of RAM, sizeofOS 

  testOS.displayMemoryBlocks();

  std::cout <<"----------------------------------\n";
  std::cout <<"SimOS NewProcess test\n";
  std::cout<< "trying to add 3000 should be false: " << std::boolalpha << testOS.NewProcess(30000, 5) << "\n";
  std::cout<< "trying to add 500 should be true: " << std::boolalpha << testOS.NewProcess(500, 7) << "\n";
  std::cout<< "trying to add 24 should be true: " << std::boolalpha << testOS.NewProcess(24, 6) << "\n";

  std::cout << "memory blocks: " << "\n";
  testOS.displayMemoryBlocks();
  std::cout << "-------\n";
  testOS.displayRunningProcess();

  std::cout << "-------\n";
  std::cout << "Display Ready Queue\n";
  testOS.displayReadyQueue();


  
  std::cout <<"----------------------------------\n";
  std::cout <<"SimOS Fork() test\n";
  std::cout << "should be True: " << std::boolalpha << testOS.SimFork() << "\n";
  std::cout << "-------\n\n";
  testOS.displayRunningProcess();

  std::cout << "-------\n\n";
  std::cout << "Display Ready Queue\n";
  testOS.displayReadyQueue();

 
  std::cout << "-------\n\n";
  std::cout << "memory blocks: " << "\n";
  testOS.displayMemoryBlocks();

  /*
  std::cout <<"----------------------------------\n";
  std::cout <<"Memory Class Test\n\n";
  Memory testMem(2048);
  std::cout << "RAM MEM amount: " << testMem.getMemAmount() << "\n";

  std::cout << "adding memeory items: \n";
  Memory::MemoryItem OS{
    0,
    1024,
    1
  };
  testMem.addToMemory(OS);
  Memory::MemoryItem item1{
    0,
    500,
    2
  };
  testMem.addToMemory(item1);
  Memory::MemoryItem item2{
    0,
    24,
    3
  };
  testMem.addToMemory(item2);
  Memory::MemoryItem item3{
    0,
    500,
    4
  };
  testMem.addToMemory(item3);
  testMem.displayMemoryBlocks();

  std::cout << "Removing PID 2 \n";
  testMem.removeMemoryItem(2);
  testMem.displayMemoryBlocks();

  std::cout << "removing PID 4\n";
  testMem.removeMemoryItem(4);
  testMem.displayMemoryBlocks();

  std::cout << "side testing add\n";
  Memory::MemoryItem item5{
    0,
    50,
    5
  };
  testMem.addToMemory(item5);
  testMem.displayMemoryBlocks();

  std::cout << "removing PID 3\n";
  testMem.removeMemoryItem(3);
  testMem.displayMemoryBlocks();
  */




 

}