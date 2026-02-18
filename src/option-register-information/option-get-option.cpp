#include "../../include/option/option-raw-metadata.hpp"
#include "../../include/option/option-implementation.hpp"
#include <unordered_map>
#include <string>

static std::unordered_map<std::string, OptionMetaData> table_option;

void GeneralOptionLog(const OptionMetaData& option){
  table_option[option.normalized_name] = option;
}

const OptionMetaData* GetOptionData(const std::string& option){
  auto its = table_option.find(option);
  if(its == table_option.end()){
    return {};
  }
  return &its->second;
}
