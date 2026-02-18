#pragma once
#include "command-raw-metadata.hpp"
#include <string>

void GeneralCommandLog();
void CreatedCommandData();
const CommandMetaData* GetCommandData(const std::string& command);
