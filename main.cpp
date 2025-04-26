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
  std::cout<< "trying to add 500 should be true: " << std::boolalpha << testOS.NewProcess(500, 5) << "\n";
  std::cout<< "trying to add 24 should be true: " << std::boolalpha << testOS.NewProcess(24, 6) << "\n";

  std::cout << "memory blocks: " << "\n";
  testOS.displayMemoryBlocks();
  std::cout << "-------\n";
  testOS.displayRunningProcess();

  std::cout << "-------\n";
  std::cout << "Display Ready Queue\n";
  testOS.displayReadyQueue();

 

}