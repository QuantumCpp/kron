#include "../../include/core-hpp/tokenization.hpp"
#include "../../include/token/token-raw-metadata.hpp"
#include <string>
#include <vector>
std::vector<Token> tokenization(const std::vector<std::string>& arguments){
  std::vector<Token> argument_raw;
  argument_raw.reserve(arguments.size());
  bool only_positional = false;
  int real_count = 0;

  for(size_t i = 0 ; i < arguments.size() ; i++){

    if(arguments[i].empty() || arguments[i] == " "){
      continue;
    }
    real_count ++;

   //Guardar primer (que no tenga guion) elemento como comando 
    if(real_count == 1 && !arguments[i].starts_with("-") && !only_positional){
      argument_raw.emplace_back(Token{
          .type = TypeToken::COMMAND,
          .name = arguments[i],
          .value = "",
      });
      continue;
    }
    
    //Detectar cuando se introdujo el "solo posicinales"
    if(arguments[i] == "--"){
      only_positional = true;
      continue;
    }

    if(arguments[i] == "-"){
      argument_raw.emplace_back(Token{
          .type = TypeToken::POSITIONAL,
          .name = arguments[i],
          .value = "",
          });
    }

    //guardar cuando se active solo posicionales 
    if(only_positional){
      argument_raw.emplace_back(Token{
          .type = TypeToken::POSITIONAL,
          .name = arguments[i],
          .value = "",
          });
      continue;
    }
    
    //guardar opciones cortas | largas | con valor explicito
    if(arguments[i].starts_with("-")){
      argument_raw.emplace_back(Token{
          .type = TypeToken::OPTION_NOT_NORMALIZED,
          .name = arguments[i],
          .value = "",
          });
      continue;
    }
    
    //Guardar los literales
    argument_raw.emplace_back(Token{
        .type = TypeToken::LITERAL,
        .name = arguments[i],
        .value = "",
        });
  }

  return argument_raw;
}
