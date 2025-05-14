//Julianne Aguilar 

#include <string>
#include <vector>
#include "Process.h"
#include "Memory.h"
#include "Disk.h"
#ifndef SIMOS_H
#define SIMOS_H

class SimOS{
  public:

    /**
    Parameterized Constructor. 
     The parameters specify the number of hard disks in the simulated computer and amount of memory.
     Disk enumeration starts from 0.
     @param: sizeOfOS specifies the size of the OS process. It has the PID of 1, priority of 0, and resides in the very beginning of memory.
    */
    SimOS( int numberOfDisks, unsigned long long amountOfRAM, unsigned long long sizeOfOS);

    /**
    Creates a new process with the specified priority in the simulated system. The new process takes place in the ready-queue or immediately starts using the CPU.
    Every process in the simulated system has a PID. Your simulation assigns PIDs to new processes starting from 2 (1 is reserved for the OS) and increments it by one for each new process. Do not reuse PIDs of the terminated processes.
    For example, the command NewProcess(1000, 5) means that a new process with priority level 5 should be created and it requires 1000 bytes of memory.
    NewProcess returns true if a new process was successfully created and false if otherwise. One of the reasons a process wasn’t created is insufficient free memory in the system.
    */
    bool NewProcess(unsigned long long size, int priority);

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

#endif