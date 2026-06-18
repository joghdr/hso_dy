#ifndef _EXPERIMENT_CLASS_H_
#define _EXPERIMENT_CLASS_H_

#include <map>
#include <string>
#include <memory>
#include <vector>

namespace hso {

  class Data;

  class Experiment {

  public:

    Experiment() = default;

    Experiment(Experiment&&) noexcept = default;

    Experiment &operator=(Experiment&&) noexcept = default;

    Experiment(const Experiment&) = delete;

    Experiment &operator=(const Experiment&) = delete;

    ~Experiment();

    bool AddDataSet(const std::string data_id, std::unique_ptr<Data> data_pointer);

    [[nodiscard]] Data* GetDataPointer(const std::string& data_id);

    [[nodiscard]] const Data* GetDataPointer(const std::string& data_id) const;

    [[nodiscard]] std::vector<Data*> GetDataPointers();

    [[nodiscard]] std::vector<const Data*> GetDataPointers() const;

  private:

    std::map<std::string, std::unique_ptr<Data>> data_sets_;

  };

}


#endif //_EXPERIMENT_CLASS_H_
