//Julianne Aguilar 
#pragma once
#include<iostream>
#include <vector>
#include <queue>

struct FileReadRequest
{
	int  PID{0};
	std::string fileName{""};
};

class Disk{
  public:

  Disk(int numOfDisks = 0);

  struct logicalDisk{
    int diskNum{-1};
    FileReadRequest currentRead = FileReadRequest();
    std::queue<FileReadRequest>filesToRead;

  };

  void readRequest(int diskNumber, std::string fileName, int pid);

  int completeRequest(int diskNumber);

  FileReadRequest getDiskRequest(int diskNumber);

  std::queue<FileReadRequest> getDiskQueue( int diskNumber);

  bool diskExist(int n);

  void terminate(int pid, int diskNumber);

  void displayDisks() const;


  private:
  int numOfDisks_;
  std::vector<logicalDisk> disks;
    
};