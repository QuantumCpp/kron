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
#include <functional>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

struct DirID {
  dev_t device;
  ino_t inode;
  bool operator==(const DirID& other) const noexcept {
    return device == other.device && inode == other.inode;
  }
};

namespace std {
  template <>
  struct hash<DirID> {
    size_t operator()(const DirID& id) const noexcept {
      size_t h1 = std::hash<dev_t>{}(id.device);
      size_t h2 = std::hash<ino_t>{}(id.inode);
      return h1 ^ (h2 << 1);
    }
  };
}

namespace {
  std::string string_perms(const std::filesystem::path& path){
    std::filesystem::perms p = std::filesystem::status(path).permissions();
    auto check = [&](std::filesystem::perms bit, char c){
      return (p & bit) != std::filesystem::perms::none ? c : '-';
    };
    std::string str;
    str += check(std::filesystem::perms::owner_read,   'r');
    str += check(std::filesystem::perms::owner_write,  'w');
    str += check(std::filesystem::perms::owner_exec,   'x');
    str += check(std::filesystem::perms::group_read,   'r');
    str += check(std::filesystem::perms::group_write,  'w');
    str += check(std::filesystem::perms::group_exec,   'x');
    str += check(std::filesystem::perms::others_read,  'r');
    str += check(std::filesystem::perms::others_write, 'w');
    str += check(std::filesystem::perms::others_exec,  'x');
    return str;
  }

  std::string size_parse(const off_t& size){
    const std::array<std::string, 5> units = {"B", "KB", "MB", "GB", "TB"};
    size_t iteration = 0;
    auto s = static_cast<double>(size);
    while(s >= 1024 && iteration < 4){
      s = s / 1024;
      iteration++;
    }
    return std::format("{:.2f}{}", s, units[iteration]);
  }

  std::string date_parse(const std::filesystem::file_time_type& raw_date){
    auto sys = std::chrono::clock_cast<std::chrono::system_clock>(raw_date);
    std::time_t t = std::chrono::system_clock::to_time_t(sys);
    std::tm* tm = std::localtime(&t);
    std::array<char, 64> buffer;
    std::strftime(buffer.data(), sizeof(buffer), "%Y-%m-%d", tm);
    return buffer.data();
  }

  void PrintInformation(
      const std::unordered_map<std::string, std::vector<std::filesystem::directory_entry>>& file_entry,
      const std::vector<std::string>& dirs_name)
  {
    for(const auto& dirs : dirs_name){
      if(!file_entry.contains(dirs)) {continue;}

      std::cout << std::format("{}/:\n", dirs);
      std::cout << std::format("{:<10} {:<10} {:<10} {:<12} {:<30} {:<8} {:<8} {:<20}\n",
          "PERMISOS", "INODE", "GRUPO", "PROPIETARIO",
          "NOMBRE", "TIPO", "TAMANO", "FECHA DE MODIFICACION");

      for(const auto& path : file_entry.at(dirs)){
        std::string type      = path.is_directory() ? "DIR" : "FIL";
        const auto perms_path = string_perms(path);
        struct stat stat_path;
        if(stat(path.path().c_str(), &stat_path) == 0){
          const auto size_path = path.is_directory()
              ? std::string("—")
              : size_parse(stat_path.st_size);
          passwd* pw = getpwuid(stat_path.st_uid);
          group*  gp = getgrgid(stat_path.st_gid);
          std::string gp_name  = gp ? gp->gr_name : "UNKNOWN";
          std::string pw_name  = pw ? pw->pw_name : "UNKNOWN";
          std::string name_cut = path.path().filename().string().size() > 30
              ? path.path().filename().string().substr(0, 30)
              : path.path().filename().string();
          auto date_path = date_parse(path.last_write_time());
          std::cout << std::format("{:<10} {:<10} {:<10} {:<12} {:<30} {:<8} {:<8} {:<8}\n",
              perms_path,
              stat_path.st_ino,
              gp_name,
              pw_name,
              name_cut + (path.is_directory() ? "/" : ""),
              type,
              size_path,
              date_path);
        }
      }
    }
  }
}

void INSPECT_HANDLER(const GroupToken& token_group){
  std::unordered_map<std::string, std::vector<std::filesystem::directory_entry>> multi_entry;
  std::vector<std::string> dirs_name;
  dirs_name.reserve(token_group.positional.size());

  if(token_group.positional.empty()){
    dirs_name.emplace_back(std::filesystem::current_path().string());
  } else {
    for(const auto& dirs : token_group.positional){
      dirs_name.emplace_back(dirs.name);
    }
  }

  bool recursive = std::ranges::any_of(token_group.options, [](const Token& t){
      return t.name == "--recursive";
  });

  std::vector<std::filesystem::directory_entry> temp_entry;

  for(const auto& dirs : dirs_name){
    if(!std::filesystem::exists(dirs)){
      std::cerr << std::format("ERROR : {} NO EXISTE O NO ES UN DIRECTORIO VALIDO\n", dirs);
      continue;
    }

    temp_entry.clear();
    std::unordered_set<DirID> visited;

    if(recursive){
      for(const auto& entry : std::filesystem::recursive_directory_iterator(dirs)){
        struct stat s;
        if(stat(entry.path().c_str(), &s) == 0){
          DirID id{ .device = s.st_dev, .inode = s.st_ino };
          if(!visited.contains(id)){
            visited.insert(id);
            temp_entry.emplace_back(entry);
          }
        }
      }
    } else {
      for(const auto& entry : std::filesystem::directory_iterator(dirs)){
        temp_entry.emplace_back(entry);
      }
    }

    multi_entry[dirs] = temp_entry;
  }

  for(const auto& option : token_group.options){
    const auto& option_data = GetOptionData(option.name);

    if(option_data->category == OptionCategory::COLLECTION) {continue;};

    if(option_data->category == OptionCategory::FILTERING ||
       option_data->category == OptionCategory::SORTING){
      for(const auto& [key, value] : multi_entry){
        FilterStruct fs;
        fs.entries = value;
        fs.context = option.value;
        std::visit([&](auto& handler){
            using T = std::decay_t<decltype(handler)>;
            if constexpr(std::is_same_v<T, FilteringProcess>){
              handler(fs);
            }
        }, option_data->hanlder);
        multi_entry[key] = fs.entries;
      }
    }
  }

  PrintInformation(multi_entry, dirs_name);
}
