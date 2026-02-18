#pragma once
#include "option-raw-metadata.hpp"

void CreatedOptionData();
void GeneralOptionLog(const OptionMetaData& option);
const OptionMetaData* GetOptionData(const std::string& name_option);
