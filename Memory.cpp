//Julianne Aguilar 

#include "Memory.h"

Memory::Memory(){

}

Memory::Memory(unsigned long long amountOfRam){
  amountOfRam_ = amountOfRam;
}

void Memory::setMemAmount(unsigned long long amountOfRam){
  amountOfRam_ = amountOfRam;
}