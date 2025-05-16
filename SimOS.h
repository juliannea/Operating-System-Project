//Julianne Aguilar 
#pragma once
#include <string>
#include <vector>
#include "Process.h"
#include "Memory.h"
#include "Disk.h"


class SimOS{
  public:


    SimOS( int numberOfDisks, unsigned long long amountOfRAM, unsigned long long sizeOfOS);

    bool NewProcess( unsigned long long size, int priority );

    //forks currently running process 
    bool SimFork();

    void SimExit();

    void SimWait();

    void DiskReadRequest( int diskNumber, std::string fileName );

    void DiskJobCompleted( int diskNumber );

    int GetCPU();

    std::vector<int> GetReadyQueue();

    MemoryUse GetMemory();

    FileReadRequest GetDisk( int diskNumber );

    std::queue<FileReadRequest> GetDiskQueue( int diskNumber );

    
    void setReadyQueuePIDs();
    
    void sort();

    void yieldCPU();

    void processPlacement(Process newProcess);

    void terminate(Process childProcess);
    //getters 
    int getProcessRunningPriority() const;

    void setProcessRunning(Process processRunning);

    //displays for testing 
    const void displayMemoryBlocks();

    void displayRunningProcess() const;

    void displayReadyQueue() const;

    void displayWaiting() const;

    void displayZombies() const;

    void displayInputOutput() const;

    void displayHardDisk() const;

    private:
      unsigned long long sizeOfOS_;
      int pid_ = 1;
      int priority_ = 0;
      unsigned long long address = 0; //starts in the very beginning of memory?
      int numberOfDisks_;

      int pidTracker_ = 1; //keep track of PID's used so know PID to assign to new process
      std::vector<Process> readyQueue;
      std::vector<int> readyQueuePIDs;
      std::vector<Process> waitingProcesses;
      std::vector<Process> zombieProcesses;
      std::vector<std::pair<Process, int> >inputOutputQueue; //pair is set as {Process, Disknumber its using}

      Process CPU_ = Process(0,0,0,0);  //process currently using CPU 
      Memory RAM_; //represents RAM
      Disk hardDisk_;

};

