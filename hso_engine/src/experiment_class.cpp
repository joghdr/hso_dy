#include <hso/experiment_class.h>
#include <hso/data_class.h>
#include <vector>
#include <string>
#include <map>


namespace hso{

  Experiment::~Experiment() = default;

  bool Experiment::AddDataSet(const std::string data_id, std::unique_ptr<Data> data_pointer){

    auto [it, loaded] = data_sets_.emplace(data_id, std::move(data_pointer));

    return loaded;

  }

  std::vector<Data*> Experiment::GetDataPointers(){

    std::vector<Data*> data_pointers;

    data_pointers.reserve(data_sets_.size());

    for (const auto &[key, value]: data_sets_) {

      data_pointers.push_back(value.get());

    }

    return data_pointers;

  }

  std::vector<const Data*> Experiment::GetDataPointers() const{

    std::vector<const Data*> data_pointers;

    data_pointers.reserve(data_sets_.size());

    for (const auto& [key, value]: data_sets_) {

      data_pointers.push_back(value.get());

    }

    return data_pointers;

  }


}//hso
