#include "../../include/error/error-id.hpp"
#include "../../include/error/error-raw-metadata.hpp"
#include "../../include/error/error-implementation.hpp"
#include <unordered_map>

static std::unordered_map<ErrorID, ErrorMetaData> table_error;

void GeneralOptionLog(const ErrorMetaData& error){
  table_error[error.error_id] = error;
}

const ErrorMetaData* GetErrorData(const ErrorID& error){
  auto its = table_error.find(error);
  if(its == table_error.end()){
    return {};
  }
  return &its->second;
}
