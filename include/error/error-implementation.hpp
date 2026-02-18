#pragma once
#include "error-raw-metadata.hpp"
#include "error-id.hpp"

void CreatedErrorData();
void GeneralErrorLog(const ErrorMetaData& error);
const ErrorMetaData* GetErrorData(const ErrorID& error); 

