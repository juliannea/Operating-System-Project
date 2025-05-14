//Julianne Aguilar
#include "Disk.h"

Disk::Disk(int numOfDisks){
  numOfDisks_ = numOfDisks;
  for(int i = 0; i < numOfDisks_ ; i++){
    logicalDisk disk{
      i, //the diskNum is equal to the order it's added to the vector
    };
    disks.push_back(disk);
  }
}

void Disk::readRequest(int diskNumber, std::string fileName, int pid){
  for(auto& disk: disks){
    if(disk.diskNum ==  diskNumber){ //find the nth disk      
      //add the files read request to I/O queue or currentRead
      FileReadRequest file{
        pid,
        fileName
      };

      if(disk.currentRead.PID == 0){ //currently not reading
        disk.currentRead = file;
      }
      else{
        disk.filesToRead.push(file);
      }
      return;
    
    }
  }
  
}

int Disk::completeRequest(int diskNumber){
  //find disk 
  if(diskExist(diskNumber)){
    int pid = disks[diskNumber].currentRead.PID;
    if(!disks[diskNumber].filesToRead.empty()){
      disks[diskNumber].filesToRead.front();
      disks[diskNumber].filesToRead.pop();
    }
    else{
      disks[diskNumber].currentRead =  FileReadRequest();
    }
    return pid;
  }
  return 0;

}

FileReadRequest Disk::getDiskRequest(int diskNumber){
  for(const auto& disk: disks){
    if(disk.diskNum == diskNumber){
      return disk.currentRead;
    }
  }
  return FileReadRequest();
}

bool Disk::diskExist(int n ){
  if (n > numOfDisks_ -1){
    return false;
  }
  return true;
}

std::queue<FileReadRequest> Disk::getDiskQueue(int diskNumber){
  for(const auto& disk: disks){
    if(disk.diskNum == diskNumber){
      return disk.filesToRead;
    }
  }
  std::queue<FileReadRequest> empty;
  return empty; //empty if disk doesnt exist with that number
}

void Disk::displayDisks() const{
  if(disks.empty()){
    std::cout << "No disks\n";
    return;
  }
  std::cout << "Disks and Read Requests: \n";
  for (const auto& disk : disks) {
    std::cout << "Disk #" << disk.diskNum << ":\n";

    std::cout << "Current Read: " << "  PID: " << disk.currentRead.PID << "  Filename: " << disk.currentRead.fileName << "\n\n";
    
    if (disk.filesToRead.empty()) {
        std::cout << "  No read requests.\n";
    } else {
      std::queue<FileReadRequest> temp = disk.filesToRead;
      std::cout << "  read requests:\n";

        while(!temp.empty()){
          std::cout << "  PID " << temp.front().PID
                      << " requested file: " << temp.front().fileName << "\n";
          temp.pop();
        }
       
    }

    
  }
}
