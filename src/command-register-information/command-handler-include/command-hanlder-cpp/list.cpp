#include "../../../../include/token/group-token.hpp"
#include "option/option-raw-metadata.hpp"
#include "option/option-implementation.hpp"
#include "token/token-raw-metadata.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <ctime>
#include <filesystem>
#include <format>
#include <iostream>
#include <ratio>
#include <string>
#include <sys/types.h>
#include <variant>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

namespace  {
  std::string string_perms(const std::filesystem::path& path){
    
    std::filesystem::perms p = std::filesystem::status(path).permissions();

    auto check = [&](std::filesystem::perms bit, char c){
      return (p & bit) != std::filesystem::perms::none ? c : '-';
    };
    std::string str;
    str += check(std::filesystem::perms::owner_read, 'r');
    str += check(std::filesystem::perms::owner_write, 'w');
    str += check(std::filesystem::perms::owner_exec, 'x');

    str += check(std::filesystem::perms::group_read, 'r');
    str += check(std::filesystem::perms::group_write, 'w');
    str += check(std::filesystem::perms::group_exec, 'x');

    str += check(std::filesystem::perms::others_read, 'r');
    str += check(std::filesystem::perms::others_write, 'w');
    str += check(std::filesystem::perms::others_exec, 'x');

    return str;
  }

  std::string size_parse(const off_t& size){
    const std::array<std::string, 5> units = {"B" , "KB" , "MB" , "GB" , "TB"};
    size_t iteration = 0;
    auto s = static_cast<double>(size);

    while(s >= 1024 && iteration < 4){
      s = s / 1024;
      iteration ++;
    }

    return std::format("{:.2f}{}", s, units[iteration]);

  }

  std::string date_parse(const std::filesystem::file_time_type& raw_date){
    auto sys = std::chrono::clock_cast<std::chrono::system_clock>(raw_date);
    std::time_t t = std::chrono::system_clock::to_time_t(sys);
    std::tm* tm = std::localtime(&t);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm);
    return buffer;
  }

  void PrintInformation(const bool& long_format, const bool& no_header_format,
      const  std::vector<std::filesystem::directory_entry>& file_entry, const std::filesystem::path& path_entry)
  {
    //Impresion basica 
    if(!long_format){
      for(const auto& entry : file_entry){
        std::cout << std::format("{}\n", entry.path().filename().string() + (entry.is_directory()? "/" : ""));
      }
    }
    else{
      if(!no_header_format){
        std::cout << std::format("{:<10} {:<10} {:<10} {:<12} {:<20} {:<8} {:<8} {:<20}\n",
                      "PERMISOS", 
                      "INODE",
                      "GRUPO",
                      "PROPIETARIO",
                      "NOMBRE" ,
                      "TIPO",
                      "TAMANO",
                      "FECHA DE MODIFICACION");
      }

      for(const auto& path : file_entry){
        std::string type = path.is_directory()? "DIR" : "FIL";
        const auto& perms_path = string_perms(path);
        struct stat stat_path;
        if(stat(path.path().c_str(), &stat_path) == 0){
          const auto& size_path = path.is_directory()? " " : size_parse(stat_path.st_size);
          passwd* pw = getpwuid(stat_path.st_uid);
          group* gp = getgrgid(stat_path.st_gid);
          
          std::string gp_name = gp ? gp->gr_name : "UNKOWN";
          std::string pw_name = pw ? pw->pw_name : "UNKOWN";

          auto date_path = date_parse(path.last_write_time());
          std::cout << std::format("{:<10} {:<10} {:<10} {:<12} {:<20} {:<8} {:<8} {:<8}\n",
              perms_path,
              stat_path.st_ino,
              gp_name,
              pw_name,
              path.path().filename().string() + (path.is_directory() ? "/" : ""), 
              type,
              size_path,
              date_path);
        }
      }
    }
  }


}

void LIST_HANDLER(const GroupToken& token_group){
  std::vector<std::filesystem::directory_entry> file_entry;
  std::filesystem::path path_entry = ".";
  bool all_format = std::ranges::any_of(token_group.options, [&](const Token& t){
      return t.name == "--all";
      });

  if(!token_group.positional.empty()){
    path_entry = token_group.positional.front().name;
  }

  for(const auto& entry : std::filesystem::directory_iterator(path_entry)){
    if(!all_format){
      if(entry.path().filename().string().starts_with('.')){
        continue;
      }
    }
    file_entry.emplace_back(entry);
    continue;
  }
  
  //Llamada basica sin opciones extra
  if(token_group.options.empty()){
    std::cout << std::format("{}\n", path_entry.string());
    for(const auto& entry : file_entry){
      std::cout << std::format("{}\n", entry.path().filename().string());
    }
  }
  else{
    bool long_format = false;
    bool no_header_format = false;

    for(const auto& option : token_group.options){
      const auto& option_data = GetOptionData(option.name);
      if(option_data->category == OptionCategory::COLLECTION){
        //en list por ahora la unica collection es --all, asi que nada
        continue;
      }
      if(option_data->category == OptionCategory::FILTERING){
        FilterStruct filter_entries;
        filter_entries.entries = file_entry;
        filter_entries.context = option.value;

        std::visit([&](auto& handler){
            using T = std::decay_t<decltype(handler)>;
            if constexpr (std::is_same_v<T,FilteringProcess>) {
             handler(filter_entries);
            }
        }, option_data->hanlder);
        file_entry = filter_entries.entries;
        continue;
      }
      
      if(option_data->category == OptionCategory::SORTING){
        FilterStruct filter_entries;
        filter_entries.entries = file_entry;
        filter_entries.context = option.value;

        std::visit([&](auto& handler){
            using T = std::decay_t<decltype(handler)>;
            if constexpr (std::is_same_v<T,FilteringProcess>) {
             handler(filter_entries);
            }
            }, option_data->hanlder);
        file_entry = filter_entries.entries;
        continue;

      }

      if(option_data->category == OptionCategory::PRESENTATION){
        if(option_data->normalized_name == "--long"){
          long_format = true;
          continue;
        }
        if(option_data->normalized_name == "--no-header"){
          no_header_format = true;
        }
      }
    }

    PrintInformation(long_format,no_header_format, file_entry,path_entry);
  }
}
