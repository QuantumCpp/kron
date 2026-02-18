#include "../../include/command/command-raw-metadata.hpp"
#include "../../include/command/command-implementation.hpp"
#include <unordered_map>
#include <string>

static std::unordered_map<std::string, CommandMetaData> table_command;

void GeneralCommandLog(const CommandMetaData& command){
  table_command[command.default_name] = command;
}

const CommandMetaData* GetCommandData(const std::string& command){
  auto its = table_command.find(command);
  if(its == table_command.end()){
    return {};
  }
  return &its->second;
}
