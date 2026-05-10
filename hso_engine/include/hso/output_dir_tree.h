#ifndef _OUTPUT_DIR_TREE_
#define _OUTPUT_DIR_TREE_

#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <memory>



namespace hso {

  class OutputDirectoryTree {

  public:

    using path = std::filesystem::path;

    OutputDirectoryTree(OutputDirectoryTree&&) noexcept = default;

    OutputDirectoryTree& operator=(OutputDirectoryTree&&) noexcept = default;

    OutputDirectoryTree(const OutputDirectoryTree&) = default;

    OutputDirectoryTree& operator=(const OutputDirectoryTree&) = default;

    ~OutputDirectoryTree() = default;

    explicit OutputDirectoryTree(const std::string &root_name,
                                 const std::vector<std::string> &subdir_names);

    const path* GetRoot() const;

    const std::vector<const path*> GetSubdir() const;

    const path* GetSubdir(const std::string &subdir_name) const;

    void Info() const;

  private:

    static std::map<std::string, path> BuildSubdirs(const std::string &root_name,
                                             const std::vector<std::string> &subdir_names);

    path root_;

    std::map<std::string, path> subdirectories_;



  };

  class ContextRegistry {

  public:

    using path = std::filesystem::path;

    ContextRegistry(ContextRegistry&&) noexcept = delete;

    ContextRegistry& operator=(ContextRegistry&&) noexcept = delete;

    ContextRegistry(const ContextRegistry&) = delete;

    ContextRegistry& operator=(const ContextRegistry&) = delete;

    ~ContextRegistry() = default;

    static ContextRegistry* GetInstance(const std::string &requested_base_name);


    OutputDirectoryTree* RequestContext(const std::string &root_name,
                                        const std::vector<std::string> &subdir_names);

    OutputDirectoryTree* RequestContext(const std::string &root_name);

    const OutputDirectoryTree* RequestContext(const std::string &root_name) const;

    void Info() const;

  private:

    explicit ContextRegistry(const std::string &requested_base_name);

    OutputDirectoryTree* CreateContext(const std::string &root_name,
                                 const std::vector<std::string> &subdir_names);

    std::string GetFullRootName(const std::string &root_name) const;

    std::string base_name_;

    std::map<std::string, std::unique_ptr<OutputDirectoryTree>> context_list_;


  };

  void MakeContext(const OutputDirectoryTree* context);


}



#endif // _OUTPUT_DIR_TREE_
