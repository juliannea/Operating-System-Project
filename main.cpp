#include <iostream>
#include "SimOS.h"
#include "Memory.h"
#include "Process.h"
#include "Disk.h"

int main(){
  std::cout <<"SimOS Constructor test\n";
  SimOS testOS(1, 2048, 1024); //disknumber, amount of RAM, sizeofOS 

  testOS.getMemoryBlocks();



}