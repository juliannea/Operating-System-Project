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
    std::vector<FileReadRequest>filesToRead;
    std::vector<FileReadRequest> filesRead;
  };

  void readRequest(int diskNumber, std::string fileName, int pid);

  void displayDisks() const;


  private:
  int numOfDisks_;
  std::vector<logicalDisk> disks;
    
};