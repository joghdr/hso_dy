#include <hso/output_dir_tree.h>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <iostream>
#include <iomanip>
#include <memory>

namespace hso {


  void MakeContext(const OutputDirectoryTree* context) {

    for (auto &subdir_path: context->GetSubdir()){

      if(std::filesystem::exists(*subdir_path)) {

        continue;

      }

      std::cout<<"Making directory "<<subdir_path->string()<<std::endl;

      std::filesystem::create_directories(*subdir_path);

    }

  }

  namespace fs = std::filesystem;

//NOTE: > > OutputDirectoryTree < <
  OutputDirectoryTree::OutputDirectoryTree(const std::string &root_name,
                                           const std::vector<std::string> &subdir_names)
    : root_(fs::absolute(fs::path(root_name))),
      subdirectories_(BuildSubdirs(root_name, subdir_names)){

        if (fs::exists(root_)) {

          std::cout<<"Directory "<<root_.string()<<" exists. Quitting."<<std::endl;

          exit(1);
        }

      }

  const fs::path* OutputDirectoryTree::GetRoot() const{

    return &root_;

  }

  const std::vector<const fs::path*> OutputDirectoryTree::GetSubdir() const{

    std::vector<const fs::path*> subdir_paths(0);

    for (auto &subdir: subdirectories_) {

      subdir_paths.push_back(&(subdir.second));

    }

    return subdir_paths;

  }

  const fs::path* OutputDirectoryTree::GetSubdir(const std::string &subdir_name) const{

    auto it = subdirectories_.find(subdir_name);

    if (it == subdirectories_.end()){

      return nullptr;

    }

    return &(it->second);
  }

  std::map<std::string, fs::path> OutputDirectoryTree::BuildSubdirs(const std::string &root_name,
                                                                    const std::vector<std::string> &subdir_names){

    std::map<std::string, fs::path> directory_map;

    fs::path root_absolute = fs::absolute(fs::path(root_name));

    for (auto &name: subdir_names){

      directory_map[name] = root_absolute / name;

    }

    return directory_map;

  }

  void OutputDirectoryTree::Info() const {

    std::cout<<"root: "<<root_.string()<<std::endl;

    for (auto &subdir: subdirectories_) {

      std::cout<<std::setw(10)<<std::right<<subdir.first<<": "<<std::setw(10)<<std::left<<subdir.second.string()<<std::endl;

    }

  }

  //NOTE: > > ContextRegistry < <
  ContextRegistry* ContextRegistry::GetInstance(const std::string &requested_base_name){

    static ContextRegistry unique_instance(requested_base_name);

    return &unique_instance;

  }

  ContextRegistry::ContextRegistry(const std::string &requested_base_name):base_name_(requested_base_name){}

  OutputDirectoryTree* ContextRegistry::RequestContext(const std::string &root_name,
                                  const std::vector<std::string> &subdir_names){

    std::string encapsulated_root_name = GetFullRootName(root_name);

    auto it = context_list_.find(encapsulated_root_name);

    if (it == context_list_.end()){

      return CreateContext(encapsulated_root_name, subdir_names);

    }

    return it->second.get();

  }

  OutputDirectoryTree* ContextRegistry::RequestContext(const std::string &root_name){

    return RequestContext(root_name, {});

  }

  const OutputDirectoryTree* ContextRegistry::RequestContext(const std::string &root_name) const{

    std::string  encapsulated_root_name = GetFullRootName(root_name);

    auto it = context_list_.find(encapsulated_root_name);

    if (it == context_list_.end()){

      return nullptr;

    }

    return it->second.get();

  }


  OutputDirectoryTree* ContextRegistry::CreateContext(const std::string &encapsulated_root_name,
                                 const std::vector<std::string> &subdir_names){


    auto [it, context_inserted] = context_list_.try_emplace(encapsulated_root_name, std::make_unique<OutputDirectoryTree>(encapsulated_root_name, subdir_names));

    std::cout<<"hso::ContextRegistry: Created new context: "<<encapsulated_root_name<<std::endl;

    return it->second.get();

  }

  std::string ContextRegistry::GetFullRootName(const std::string &root_name) const{

    fs::path base(base_name_);

    fs::path root(root_name);

    return (base / root).string();

  }

  void ContextRegistry::Info() const{
    std::cout<<"- - ContextRegistry info - -"<<std::endl;
    std::cout<<" base_name_: "<<base_name_<<std::endl;
    std::cout<<" contexts present:"<<std::endl;
    for (auto &context: context_list_){
      std::cout<<" - "<<context.first<<" - "<<std::endl;
      context.second->Info();
    }




  }


}
