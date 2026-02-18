#pragma once
#include <cstdint>

enum class ErrorID : std::uint8_t{
  NOT_ARGUMENTS, //No se introdujeron argumentos
  COMMAND_NOT_FOUND, //No se encontro un comando valido
  OPTION_NOT_FOUND, //No se encontro una opcion valida
};
