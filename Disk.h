//Julianne Aguilar 
#pragma once
#include<iostream>
#include <vector>

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
    std::vector<FileReadRequest>filesToRead;

  };

  void readRequest(int diskNumber, std::string fileName, int pid);

  int completeRequest(int diskNumber);

  FileReadRequest getDiskRequest(int diskNumber);

  bool diskExist(int n);

  void displayDisks() const;


  private:
  int numOfDisks_;
  std::vector<logicalDisk> disks;
    
};