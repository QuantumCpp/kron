#include "../../include/option/option-raw-metadata.hpp"
#include "../../include/option/option-implementation.hpp"
#include <functional>
#include "../../include/token/token-raw-metadata.hpp"
#include <unordered_map>
#include <string>

static std::unordered_map<std::string, OptionMetaData, transparent_hash, std::equal_to<>> table_option;

void GeneralOptionLog(const OptionMetaData& option){
  table_option[option.normalized_name] = option;
  table_option[option.alias_name] = option;
}

const OptionMetaData* GetOptionData(std::string_view option){
  auto its = table_option.find(option);
  if(its == table_option.end()){
    return {};
  }
  return &its->second;
}
