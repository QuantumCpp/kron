#pragma once
#include "error-raw-metadata.hpp"
#include "error-id.hpp"

void GeneralErrorLog();
void CreatedErrorData(const ErrorMetaData& Error);
const ErrorMetaData* GetErrorData(const ErrorID& Error_ID); 

