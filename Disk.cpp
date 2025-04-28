#include "Disk.h"

Disk::Disk(int numOfDisks){
  numOfDisks_ = numOfDisks;
  for(int i = 1; i < numOfDisks_ + 1; i++){
    logicalDisk disk{
      i, //the diskNum is equal to the order it's added to the vector
    };
    disks.push_back(disk);
  }
}

void Disk::readRequest(int diskNumber, std::string fileName, int pid){
  for(auto& disk: disks){
    if(disk.diskNum ==  diskNumber){
      //add the files read request to the collection of files read from the disk
      FileReadRequest file{
        pid,
        fileName
      };
      
      disk.filesToRead.push_back(file);
      return;
    
    }
  }
  
}

int Disk::completeRequest(int diskNumber){
  //find disk 
  for(auto& disk: disks){
    if(disk.diskNum == diskNumber){
      //assume FIFO complete the request and the beggining of the vector 
      disk.filesRead.push_back(disk.filesToRead[0]);
      int pid = disk.filesToRead[0].PID; //pid of process request that was completed
      disk.filesToRead.erase(disk.filesToRead.begin());
      return pid;
    }
  }
  return 0;
}

void Disk::displayDisks() const{
  if(disks.empty()){
    std::cout << "No disks\n";
    return;
  }
  std::cout << "Disks and Read Requests: \n";
  for (const auto& disk : disks) {
    std::cout << "Disk #" << disk.diskNum << ":\n";
    
    if (disk.filesToRead.empty()) {
        std::cout << "  No read requests.\n";
    } else {
      std::cout << "  \nread requests:\n";
        for (const auto& request : disk.filesToRead) {
            std::cout << "  PID " << request.PID 
                      << " requested file: " << request.fileName << "\n";
        }
    }

    if(disk.filesRead.empty()){
      std::cout <<"  No files read request completed\n";

    }
    else{
      for(const auto& request: disk.filesRead){
        std::cout << "  \n  read requests completed: \n";
        std::cout << "  PID " << request.PID 
                      << " requested file: " << request.fileName << "\n";

      }
    }
  }
}
