#pragma once

#include "error-id.hpp"
#include <functional>
#include <variant>
#include <vector>
#include <string>

struct WithDetails{
  std::string trigger;
  std::vector<std::string> detail;
};

struct WithoutDetails{
  std::string trigger;
};

using WithDetailsHanlder = std::function<bool (const WithDetails& )>;
using WithoutDetailsHandler = std::function<bool (const WithoutDetails& )>;

using Errorhandler = std::variant<
  WithDetailsHanlder,
  WithoutDetailsHandler
>;

struct ErrorMetaData{
  ErrorID error_id;
  Errorhandler handler;
};


