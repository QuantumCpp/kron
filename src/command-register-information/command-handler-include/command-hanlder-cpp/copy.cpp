#include "../../../../include/token/group-token.hpp"
#include "../../../../include/error/error_hanlder.hpp"
#include "option/option-implementation.hpp"
#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <iterator>
#include <queue>
#include <filesystem>
#include <thread>
#include <mutex>
#include <unordered_set>
#include <vector>
#include <sys/stat.h>

const int MAX_THREAD = 9;

enum class TypeEntry : uint8_t{
  Directory = 0,
  File = 1,
  SymLink = 2,
};

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

    void dry_run(const std::vector<std::filesystem::directory_entry>& entries_origin, 
      const std::filesystem::directory_entry& destinatary_dirs, const std::filesystem::directory_entry& origin_dirs){
      for(const auto& entry : entries_origin){
        std::filesystem::path path = destinatary_dirs.path() / entry.path().lexically_relative(origin_dirs); 
        if(std::filesystem::exists(path)){
          std::cout << std::format("[DRY-RUN] CONFLICT : {}\n", path.filename().string());
          continue;
        }
        if(entry.is_directory()){
          std::cout << std::format("[DRY-RUN] CREATE DIR : {}\n", path.filename().string());
          continue;
        }
        if(entry.is_symlink()){
          std::cout << std::format("[DRY-RUN CREATE SYMLINK] : {}\n", path.filename().string());
          continue;
        }
        if(entry.is_regular_file()){
          std::cout << std::format("[DRY-RUN] COPY : {}\n", path.filename().string());
        }
      }
    }



  void copy_dir_process(const std::vector<std::filesystem::directory_entry>& entries_origin, 
      const std::filesystem::directory_entry& destinatary_dirs,const std::filesystem::directory_entry& origin_dirs,
      const std::unordered_set<std::string>& collect_option){

    std::filesystem::copy_options copy_option = std::filesystem::copy_options::none;
    
    if(collect_option.contains("--dry-run")){
      dry_run(entries_origin, destinatary_dirs, origin_dirs);
      return;
    }

    for(const auto& option : collect_option){
      if(option == "--force"){
        copy_option = std::filesystem::copy_options::overwrite_existing;
      }
      if(option == "--skip-existing"){
        copy_option = std::filesystem::copy_options::skip_existing;
      }
    }

    bool repeat = false;

    for(const auto& entry : entries_origin){
      std::filesystem::path path(destinatary_dirs.path() /  entry.path().lexically_relative(origin_dirs));
      bool exist_path = entry.is_symlink() ? std::filesystem::exists(std::filesystem::symlink_status(path))  : std::filesystem::exists(path);
      if(collect_option.contains("--no-overwrite") && exist_path){
        std::cout << std::format("[NO-OVERWRITE] {}\n", path.filename().string());
        continue;
      }

        if(exist_path && !repeat && 
        (!collect_option.contains("--force") && !collect_option.contains("--skip-existing"))){
          int option = 0;
          while(option == 0){
            std::cout << std::format("The entry {} already exists\n", path.filename().string());
            std::cout << "1. Skip entry \n";
            std::cout << "2. Update entry\n";
            std::cout << "3. Overwrite entry\n";
            std::cout << "If you enter -[election], the process will be performed for the remaining entries where this occurs.\n";
            std::cin >> option;
          
            switch (option) {
              case -1:
                copy_option = std::filesystem::copy_options::skip_existing;
                repeat = true;
                break;
              case -2:
                copy_option = std::filesystem::copy_options::update_existing;
                repeat = true;
                break;
              case -3:
                copy_option = std::filesystem::copy_options::overwrite_existing;
                repeat = true;
                break;

              case 1:
                copy_option = std::filesystem::copy_options::skip_existing;
                break;
              case 2:
                copy_option = std::filesystem::copy_options::update_existing;
                break;
              case 3:
                copy_option = std::filesystem::copy_options::overwrite_existing;
                break;
              default:
                option = 0;
                break;
            }
        }
      }

      std::filesystem::path parent_path = path.parent_path();
      if(!std::filesystem::exists(parent_path)){
        std::filesystem::create_directories(parent_path);
      }

      if(entry.is_directory()){
        if(copy_option == std::filesystem::copy_options::skip_existing || 
            copy_option == std::filesystem::copy_options::overwrite_existing ||
            copy_option == std::filesystem::copy_options::none){
          if(collect_option.contains("--verbose")) 
          { 
            std::cout << std::format("[CREATE DIR] : {}\n",path.filename().string() );
          }
          std::filesystem::create_directory(path);
          continue;
        }
        if(copy_option == std::filesystem::copy_options::update_existing){
          
          if(collect_option.contains("--verbose")) 
          {
            std::cout << std::format("[UPDATE DIR] : {}\n",path.filename().string() );
          }


          std::filesystem::remove_all(path);
          std::filesystem::create_directory(path);
          continue;
        }
      }
      if(entry.is_symlink()){
        if(copy_option == std::filesystem::copy_options::skip_existing){
          if(collect_option.contains("--verbose")) 
          {
            std::cout << std::format("[SKIP SYMLINK] : {}\n",path.filename().string() );
          }

          continue;
        }
        if(copy_option == std::filesystem::copy_options::update_existing || 
            copy_option == std::filesystem::copy_options::overwrite_existing){
          if(collect_option.contains("--verbose")) 
          {
            std::cout << std::format("[UPDATE SYMLINK] : {}\n",path.filename().string() );
          }

          std::filesystem::remove(path);
          std::filesystem::copy_symlink(entry.path(), path);
          continue;
        }
        if(collect_option.contains("--verbose")) 
        {
          std::cout << std::format("[CREATE SYMLINK] : {}\n",path.filename().string() );
        }

        std::filesystem::copy_symlink(entry.path(),path);
        continue;
      } 
      if(entry.is_regular_file()){
          if(collect_option.contains("--verbose")) 
          {
            std::cout << std::format("[COPY FILE] : {}\n",path.filename().string() );
          }

        std::filesystem::copy_file(entry.path() , path, copy_option);
      }
  }}
}

void COPY_HANLDER(const GroupToken& token_group){
  std::queue<std::filesystem::directory_entry> dirs_name_queque;
  std::filesystem::directory_entry origin_dirs = std::filesystem::directory_entry(token_group.positional.front().name);
  std::filesystem::directory_entry destinatary_dirs = std::filesystem::directory_entry(token_group.positional.back().name);
  std::vector<std::filesystem::directory_entry> extract_dirs_origin;
  std::unordered_set<DirID> visited;


  auto depth_it = std::ranges::find_if(token_group.options, [](const auto& token){
      return token.name == "--depth"; 
  });
  int depth_value = 0;

  if(depth_it != token_group.options.end() && !depth_it->value.empty()){
    depth_value = std::stoi(depth_it->value);
  }

  std::unordered_set<std::string> collect_option;

  for(const auto& option : token_group.options){
    if(option.name == "--fields"){
      continue;
    }
    collect_option.insert(option.name);
  }

  if(collect_option.contains("--depth") && !collect_option.contains("--recursive")){
    collect_option.insert("--recursive");
  }

  if((origin_dirs == destinatary_dirs)){
    EQUAL_DIRECTION();
    return;
  }
  
  
  if(!origin_dirs.exists()){
    ORIGIN_DIRECTORY_NOT_VALITED(origin_dirs.path().filename().string());
    return;
  }
  

  if(!std::filesystem::exists(destinatary_dirs)){
    if(collect_option.contains("--dry-run")){
      std::cout << std::format("[DRY-RUN] CREATE DIR : {}\n", destinatary_dirs.path().filename().string());
    }
    else{
      if(collect_option.contains("--verbose")){
        std::cout << std::format("[CREATE DIR] : {}\n", destinatary_dirs.path().filename().string());
      }
      std::filesystem::create_directory(destinatary_dirs);
    }
  }

  dirs_name_queque.emplace(origin_dirs);
  std::mutex security_dirs_name;
  std::mutex security_entry_data;
  std::vector<std::thread> threads;
  std::condition_variable condition;
  std::atomic<bool> not_more_elements = false;
  size_t  thread_active = 0;

  threads.reserve(MAX_THREAD);
  if(collect_option.contains("--recursive")){
    for(size_t i = 0 ; i < MAX_THREAD ; i++ ){
      threads.emplace_back([&]{

        try{
        while(!not_more_elements){
            std::unique_lock<std::mutex> auto_lock(security_dirs_name);
            condition.wait(auto_lock, [&]{return !dirs_name_queque.empty() || not_more_elements;} );
            if(not_more_elements){
              break;
            }
            auto entry(dirs_name_queque.front());
            dirs_name_queque.pop();
            thread_active++;
            auto_lock.unlock();
            
            if(depth_it != token_group.options.end()){
              if(!entry.is_regular_file()){
                for(auto entry_its = std::filesystem::recursive_directory_iterator(entry, std::filesystem::directory_options::none);
                    entry_its != std::filesystem::recursive_directory_iterator();
                    ++entry_its)
                {
                  if(entry_its.depth() <= depth_value ){
                    struct stat s;
                    if(lstat(entry_its->path().c_str(), &s) == 0){
                      DirID id{ .device = s.st_dev, .inode = s.st_ino };
                        {
                          std::lock_guard lock(security_entry_data);
                          if(!visited.contains(id)){
                            extract_dirs_origin.push_back(*entry_its);
                            visited.insert(id);
                          }
                        }
                    }
                  }
                }
              }
              else{
                {
                  std::lock_guard lock(security_entry_data);
                  extract_dirs_origin.push_back(entry);
                }
              }
            }
            else{
              if(!entry.is_regular_file()){
                for(const auto& entry_its : std::filesystem::directory_iterator(entry)){
                  if(!entry_its.is_directory()){
                    struct stat s;
                    if(lstat(entry_its.path().c_str(), &s) == 0){
                      DirID id{ .device = s.st_dev, .inode = s.st_ino };
                      {
                        std::lock_guard lock(security_entry_data);
                        if(!visited.contains(id)){
                          extract_dirs_origin.emplace_back(entry_its);
                          visited.insert(id);
                        }
                      }
                    }
                  }
                  else{
                    {
                      std::lock_guard<std::mutex>lock(security_dirs_name);
                      dirs_name_queque.push(entry_its);
                    }
                    condition.notify_all();
                    {
                      // FIX: lógica estaba invertida — registrar si NO está en visited,
                      // igual que la rama de no-directorios. Antes decía contains(id)
                      // como condición para insertar, así que nunca grababa nada
                      // (ningún directorio puede estar en visited antes de ser procesado).
                      struct stat s;
                      if(lstat(entry_its.path().c_str(), &s) == 0){
                        DirID id = {.device = s.st_dev , .inode = s.st_ino};
                        std::lock_guard lock(security_entry_data);
                        if(!visited.contains(id)){
                          extract_dirs_origin.emplace_back(entry_its);
                          visited.insert(id);
                        }
                      }
                    }
                  }
                }
              }
              else{
                {
                  std::lock_guard lock(security_entry_data);
                  extract_dirs_origin.push_back(entry);
                }
              }
            }
            {
            std::lock_guard lock(security_dirs_name);
              thread_active--;
              if(thread_active == 0 && dirs_name_queque.empty()){
                not_more_elements = true;
              }
            }
            condition.notify_all();
          }
        }catch(std::filesystem::filesystem_error& e){
            {
              std::lock_guard lock(security_dirs_name);
              thread_active--;
              if(thread_active == 0 && dirs_name_queque.empty()){
                not_more_elements = true;
              }
            }
            std::cerr << std::format("[ERROR] {}\n", e.what());
            condition.notify_all();
        }
      });
    }
    for(auto& its : threads){
      its.join();
    }
    //ordenar elementos
    std::vector<std::tuple<TypeEntry,ptrdiff_t, std::filesystem::directory_entry>> temp_sort;
    
    for(const auto& entry : extract_dirs_origin){
      TypeEntry entry_type;
      // FIX: is_symlink() antes que is_directory(). Un symlink a directorio
      // devuelve true para ambos predicados; clasificarlo como Directory lo
      // ordena con valor 0 y se crea antes que los directorios reales que
      // necesita como destino, causando ENOENT o ELOOP.
      if(entry.is_symlink()){
        entry_type = TypeEntry::SymLink;
      }
      else if(entry.is_directory()){
        entry_type = TypeEntry::Directory;
      }
      else{
        entry_type = TypeEntry::File;
      }
      temp_sort.push_back({entry_type,std::ranges::distance(entry.path().begin(), entry.path().end()), entry});
    }
    //ordenar elementos
    std::ranges::sort(temp_sort, [](const auto& a, const auto& b)
        {
          return std::tuple(std::get<0>(a) , std::get<1>(a), std::get<2>(a).path().filename()) < std::tuple(std::get<0>(b), std::get<1>(b), std::get<2>(b).path().filename()) ;
        });
    std::ranges::transform(temp_sort, extract_dirs_origin.begin(), [](const auto& entry){
        return std::get<2>(entry);
        });
  }
  else{
    if(origin_dirs.is_regular_file()){
      extract_dirs_origin.emplace_back(origin_dirs);
    }
    else{
      for(const auto& entry_its : std::filesystem::directory_iterator(origin_dirs)){
        extract_dirs_origin.emplace_back(entry_its);
      }
    }
  }

  for(const auto& option : token_group.options){
    const auto& option_data = GetOptionData(option.name);

    if(option_data->category == OptionCategory::COLLECTION) { continue; }

    if(option_data->category == OptionCategory::FILTERING || option_data->category == OptionCategory::SORTING){
        FilterStruct fs;
        fs.entries = extract_dirs_origin;
        fs.context = option.value;
        std::visit([&](auto& handler){
          using T = std::decay_t<decltype(handler)>;
          if constexpr(std::is_same_v<T, FilteringProcess>){
            handler(fs);
          }
        }, option_data->hanlder);
        extract_dirs_origin = fs.entries;
    }
  }

  copy_dir_process(extract_dirs_origin, destinatary_dirs, origin_dirs,collect_option);
}
