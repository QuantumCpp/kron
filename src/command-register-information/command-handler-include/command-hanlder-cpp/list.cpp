#include "../../../../include/token/group-token.hpp"
#include "../../../../include/option/option-raw-metadata.hpp"
#include "../../../../include/option/option-implementation.hpp"
#include "../../../../include/token/token-raw-metadata.hpp"
#include <format>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>

struct HealthFlag {
  uint8_t code;
  uint8_t level;       // 1=info, 2=medium, 3=high
};

struct FileEntry {
  ino_t inode;
  off_t size;
  mode_t mode;
  nlink_t nlinks;

  uint8_t is_directory : 1;
  uint8_t is_symlink : 1;
  uint8_t symlink_broken : 1;
  uint8_t has_health : 1;
  uint8_t reserved : 4;

  std::string name;
  std::unique_ptr<std::vector<HealthFlag>> healt;

};

namespace {
  bool exist_entry(const std::string& entry){
    struct stat info_file;
    if(stat(entry.c_str(), &info_file) != 0){
      switch (errno) {
        case EACCES:
          std::cout << std::format("Access denied\n");
          break;
        case ENOTDIR:
          std::cout << std::format("Not is directory\n");
          break;
      }
    }
    return S_ISDIR(info_file.st_mode);
  }
}

void LIST_HANDLER(const GroupToken& token_group){
  if(exist_entry(token_group.positional.front().name)){
    std::cout << ("good\n");
  }
}
