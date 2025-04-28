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
  std::cout <<"----------------------------------\n";
  std::cout <<"SimOS Exit() and Wait() test\n";
  
  /*
  testOS.SimWait();
  std::cout << "Case for Parent is terminated PID 2 and child PID 4 should be terminated\n";
  testOS.displayRunningProcess();
  testOS.displayReadyQueue();
  testOS.displayMemoryBlocks();
  testOS.displayZombies();
  testOS.displayWaiting();
  */
  
  /*
  std::cout << "Child exits before parent calls wait\n";
  testOS.setProcessRunning(Process(4, 7, 500, 3, 2));
  testOS.displayRunningProcess();
  std::cout << "\n----------------\n";
  std::cout << "Child calls exit test, child should turn to zombie, next process in ready queue goes\n";
  testOS.SimExit();
  testOS.displayRunningProcess();
  testOS.displayReadyQueue();
  testOS.displayMemoryBlocks();
  testOS.displayWaiting();
  testOS.displayZombies();
  std::cout << "\n----------------\n";
  std::cout << "Parent calls wait, but child is the zombie so parent continues and zombie child terminates\n";
  Process testP(2, 7, 500, 3, -1);
  testP.addChild(Process(4,7,500,3,2));
  testOS.setProcessRunning(testP);

  testOS.SimWait();
  testOS.displayRunningProcess();
  testOS.displayReadyQueue();
  testOS.displayMemoryBlocks();
  testOS.displayWaiting();
  testOS.displayZombies();
  */
 std::cout << "Case parent is waiting so child terminates and nothing in waiting and process queue after\n\n";

 std::cout << "Parent waits test, parent in waring queue and next in ready queue enters: \n";
 testOS.SimWait();
 testOS.displayRunningProcess();
 testOS.displayReadyQueue();
 testOS.displayMemoryBlocks();
 testOS.displayWaiting();
 testOS.displayZombies();
 std::cout << "-----------------------\n\n";
 std::cout << "After parent is set to wait, its child runs and then exit test. Child should terminate from memory\n";
 testOS.SimExit();
 testOS.displayRunningProcess();
 testOS.displayReadyQueue();
 testOS.displayMemoryBlocks();
 testOS.displayWaiting();
 testOS.displayZombies();

 std::cout <<"----------------------------------\n";
 std::cout <<"Get CPU test\n";
 std::cout << "currently running on the CPU: " << testOS.GetCPU() << "\n";

 std::cout << "----------------\n";
 std::cout << "GetReadyQueue test\n";
 for(const auto& pid : testOS.GetReadyQueue()){
  std::cout << "PID: " << pid << "\n";

 }
 int i = 0;
 std::cout << "----------------\n";
 std::cout << "GetMemory test: \n";
 for (const auto& block : testOS.GetMemory()) {
  std::cout << "index: " << i << ", Address: 0x" << std::hex << block.itemAddress << std::dec
            << ", Size: " << block.itemSize
            << ", PID: " << block.PID << "\n";
  i++;
}



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