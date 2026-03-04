#include "../../../include/error/error-raw-metadata.hpp"
#include "handler_error.hpp"
#include <iostream>
#include <string>
#include <format>

void NOT_ARGUMENTS_HANLDER(const WithoutDetails& trigger){
  std::cerr << "NO SE INTRODUJO ARGUMENTOS\n";
}
void COMMAND_NOT_FOUND_HANDLER(const WithoutDetails& trigger){
  std::string error_message = std::format("ERROR : COMANDO [{}] NO ENCONTRADO" , trigger.trigger);
  std::cerr << error_message;
} 
void OPTION_NOT_FOUND_HANDLER(const WithoutDetails& trigger){
  std::string error_message = std::format("ERROR : OPCION [{}] NO ENCONTRADA" , trigger.trigger);
  std::cerr << error_message;

} 
void OPTION_NEED_VALUE_HANDLER(const WithoutDetails& trigger){
  std::string error_message = std::format("ERROR : OPCION [{}] NECESITA UN VALOR" , trigger.trigger);
  std::cerr << error_message;
} 
