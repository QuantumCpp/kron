#include "../../include/error/error-implementation.hpp"
#include "../../include/error/error-raw-metadata.hpp"
#include "../../include/error/error-id.hpp"
#include "handler_error/handler_error.hpp"

void CreatedErrorData(){
  ErrorMetaData NOT_ARGUMENT = ErrorMetaData{
    .error_id = ErrorID::NOT_ARGUMENTS,
    .handler = WithoutDetailsHandler(NOT_ARGUMENTS_HANLDER),
  } ;

  ErrorMetaData COMMAND_NOT_FOUND = ErrorMetaData{
    .error_id = ErrorID::COMMAND_NOT_FOUND,
    .handler = WithoutDetailsHandler(COMMAND_NOT_FOUND_HANDLER),
  } ;
  ErrorMetaData OPTION_NOT_FOUND = ErrorMetaData{
    .error_id = ErrorID::OPTION_NOT_FOUND,
    .handler = WithoutDetailsHandler(OPTION_NOT_FOUND_HANDLER),
  } ;

  ErrorMetaData OPTION_NEED_VALUE = ErrorMetaData{
    .error_id = ErrorID::OPTION_NEED_VALUE,
    .handler = WithoutDetailsHandler(OPTION_NEED_VALUE_HANDLER),
  } ;

}
