#include "../../../../include/token/group-token.hpp"
#include "../../../../include/error/error_hanlder.hpp"
#include "option/option-implementation.hpp"
#include <algorithm>
#include <cstddef>
#include <format>
#include <iostream>
#include <iterator>
#include <queue>
#include <filesystem>
#include <thread>
#include <mutex>

const int MAX_THREAD = 9;

struct bool_option_avaible{

};

namespace {
  void copy_dir_process(const std::vector<std::filesystem::directory_entry>& entries_origin, 
      const std::filesystem::directory_entry& destinatary_dirs, std::filesystem::directory_entry& origin_dirs){
    for(const auto& entry : entries_origin){
      std::filesystem::path path = destinatary_dirs.path() /  std::filesystem::relative(entry.path(), origin_dirs.path());
      std::filesystem::copy_options copy_option = std::filesystem::copy_options::none;
      int option = 0;

      if(std::filesystem::exists(path)){
        while(option == 0){
          std::cout << std::format("The entry {} already exists", path.filename().string());
          std::cout << "1. Skip entry\n";
          std::cout << "2. Update entry\n";
          std::cout << "3. Overwrite entry\n";
          std::cin >> option;
          
          switch (option) {
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
      std::filesystem::copy(entry.path(), path, copy_option);
  }}
}

void COPY_HANLDER(const GroupToken& token_group){
  std::queue<std::filesystem::directory_entry> dirs_name_queque;
  std::filesystem::directory_entry origin_dirs = std::filesystem::directory_entry(token_group.positional.front().name);
  std::filesystem::directory_entry destinatary_dirs = std::filesystem::directory_entry(token_group.positional.back().name);
  std::vector<std::filesystem::directory_entry> extract_dirs_origin;
  
  auto depth_it = std::ranges::find_if(token_group.options, [](const auto& token){
      return token.name == "--depth"; 
  });
  int depth_value = 0;

  if(depth_it != token_group.options.end() && !depth_it->value.empty()){
    depth_value = std::stoi(depth_it->value);
  }


  bool recursive = std::ranges::any_of(token_group.options, [](const auto& token){
        return token.name == "--recursive";
  });
  bool verbose = std::ranges::any_of(token_group.options, [](const auto& token){
        return token.name == "--verbose";
  });
  bool force = std::ranges::any_of(token_group.options, [](const auto& token){
      return token.name == "--force";
  });
  bool preserve = std::ranges::any_of(token_group.options, [](const auto& token){
      return token.name == "--preserve";
  });
  bool dry_run = std::ranges::any_of(token_group.options, [](const auto& token){
      return token.name == "--dry-run";
  });

  if((origin_dirs == destinatary_dirs)){
    EQUAL_DIRECTION();
    return;
  }

  if(!std::filesystem::exists(destinatary_dirs)){
    std::filesystem::create_directory(destinatary_dirs);
  }

  if(!std::filesystem::exists(origin_dirs)){
    //anadir error de direccion inexistente objetivo inexistente
    return;
  }
  dirs_name_queque.emplace(origin_dirs);
  std::vector<std::filesystem::directory_entry> dirs_entry;
  size_t max_thread = std::thread::hardware_concurrency() > MAX_THREAD ? MAX_THREAD : std::thread::hardware_concurrency();
  size_t min_thread = token_group.positional.empty() ? 1 : token_group.positional.size();
  size_t n_thread = std::min(min_thread,max_thread);
  std::mutex security_dirs_name;
  std::mutex security_entry_data;
  std::vector<std::thread> threads;
  threads.reserve(n_thread);
  if(recursive){
    for(size_t i = 0 ; i < n_thread ; i++ ){
      threads.emplace_back([&]{
          bool not_empty_queque = true;

          security_dirs_name.lock();
          not_empty_queque = !dirs_name_queque.empty();
          security_dirs_name.unlock();

          while(not_empty_queque){
            security_dirs_name.lock();
            if(dirs_name_queque.empty()){
              security_dirs_name.unlock();
              break;
            }
            const auto& entry(dirs_name_queque.front());
            dirs_name_queque.pop();
            security_dirs_name.unlock();
            if(depth_it != token_group.options.end()){
              for(auto entry_its = std::filesystem::recursive_directory_iterator(entry);
                  entry_its != std::filesystem::recursive_directory_iterator();
                  ++entry_its)
              {
                if(entry_its.depth() <= depth_value ){
                  security_entry_data.lock();
                  extract_dirs_origin.push_back(*entry_its);
                  security_entry_data.unlock();
                  
                }
              }
            }
            else{
              for(const auto& entry_its : std::filesystem::directory_iterator(entry)){
                if(!entry.is_directory()){
                  security_entry_data.lock();
                  extract_dirs_origin.emplace_back(entry_its);
                  security_entry_data.unlock();
                }
                else{
                  security_dirs_name.lock();
                  dirs_name_queque.push(entry_its);
                  security_dirs_name.unlock();
                  
                  security_entry_data.lock();
                  extract_dirs_origin.emplace_back(entry_its);
                  security_entry_data.unlock();
                }
              }
            }
          }
      });
    }
    for(auto& its : threads){
      its.join();
    }
    //ordenar elementos
    std::vector<std::pair<ptrdiff_t, std::filesystem::directory_entry>> temp_sort;
    
    for(const auto& entry : extract_dirs_origin){
      temp_sort.push_back({std::ranges::distance(entry.path().begin(), entry.path().end()), entry});
    }
    //ordenar elementos
    std::ranges::sort(temp_sort, [](const auto& a, const auto& b)
        {
          return std::pair(a.first , a.second.path().filename()) < std::pair(b.first, b.second.path().filename());
        });
    std::ranges::transform(temp_sort, extract_dirs_origin.begin(), [](const auto& entry){
        return entry.second;
        });
  }
  else{
    for(const auto& entry_its : std::filesystem::directory_iterator(origin_dirs)){
      extract_dirs_origin.emplace_back(entry_its);
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

  copy_dir_process(extract_dirs_origin, destinatary_dirs, origin_dirs);
}


