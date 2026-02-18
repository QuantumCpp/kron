#pragma once
#include "command-raw-metadata.hpp"
#include <string>

void CreatedCommandData();
void GeneralCommandLog(const CommandMetaData& command);
const CommandMetaData* GetCommandData(const std::string& command);
