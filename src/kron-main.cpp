#include "command/command-implementation.hpp"
#include <iostream>
#include <string>
#include <vector>


int main(int argc , char* argv[]){
  CreatedCommandData();

  std::vector<std::string> arguments_raw(argv + 1 , argv + argc);

  std::cout << "kron - ejectuado\n";
  return 0 ;
}
