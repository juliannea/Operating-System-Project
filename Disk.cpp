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
  if(pid > 1 && diskExist(diskNumber)){
    FileReadRequest file{
      pid,
      fileName
    };
    if(disks[diskNumber].currentRead.PID == 0) {
      //currently not reading
      disks[diskNumber].currentRead = file;
    }
    else{
      disks[diskNumber].filesToRead.push(file); //add to a
    }
  }
  
}

int Disk::completeRequest(int diskNumber){
  //find disk 
  if(diskExist(diskNumber)){
    int pid = disks[diskNumber].currentRead.PID;
    if(!disks[diskNumber].filesToRead.empty()){
      disks[diskNumber].currentRead = disks[diskNumber].filesToRead.front();
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
  if (n > numOfDisks_ -1 || n < 0){
    return false;
  }
  return true;
}

void Disk::terminate(int pid, int diskNumber){
  if(!diskExist(diskNumber)){
    return; 
  }
  std::queue<FileReadRequest> temp;

  //case process we're deleting is the current read
  if(disks[diskNumber].currentRead.PID == pid){
    //std::cout << "DISK T Case 1" << disks[diskNumber].currentRead.PID << "\n";
    //remove from current read and replace current read with next one in the q
    if(!disks[diskNumber].filesToRead.empty()){
      //if q not empty replace with front of q 
      disks[diskNumber].currentRead = disks[diskNumber].filesToRead.front();
      disks[diskNumber].filesToRead.pop();
    }
    else{
      disks[diskNumber].currentRead =  FileReadRequest();
    }
  }
  else{
    //not the current read
    while(!disks[diskNumber].filesToRead.empty()){
      int current = disks[diskNumber].filesToRead.front().PID; //current PID of process in q we're looking at 
      if(current != pid){
        temp.push(disks[diskNumber].filesToRead.front());
      }
      disks[diskNumber].filesToRead.pop();
    }

    disks[diskNumber].filesToRead = temp;
    if(!temp.empty()){
      disks[diskNumber].currentRead = disks[diskNumber].filesToRead.front();
      disks[diskNumber].filesToRead.pop();
      
    }
  }
  
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
      std::cout << "read requests:\n";

        while(!temp.empty()){
          std::cout << "  PID " << temp.front().PID
                      << " requested file: " << temp.front().fileName << "\n";
          temp.pop();
        }
    }
    std::cout << "\n";

    
  }
}
