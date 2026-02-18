#pragma once
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <variant>
#include <vector>
#include <any>

enum class TypeDataReceived : std::uint8_t{
  DATE,
  SIZE,
  EXTENSION,
  NONE,
};

struct FilterStruct{
  std::vector<std::filesystem::directory_entry> entry_element;
  std::any filter_contex;
};

/*
 El FilteringProcess incluye todos los procesos que modifican el listado de elementos crudos
 obtenidos de las rutas indicadas (o elementos) :
 entre estos estan las opciones como : 
  - recursiva
  - directorios que empiecen con punto
  - filtrado por tamano | fecha | permisos | 
*/

using FilteringProcess = std::function<void (FilterStruct& )>;  

using OptionHandler = std::variant<
  std::monostate, //para especificar aquellas opciones que no necesitan implementacion especial 
  FilteringProcess
>;

struct OptionMetaData{
  std::string normalized_name;
  std::string alias_name;
  std::vector<std::string> conflict_name;
  TypeDataReceived data_type;
  OptionHandler hanlder;
};
