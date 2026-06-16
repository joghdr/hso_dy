#include <hso/experiment_loader.h>
#include <hso/experiment_class.h>
#include <hso/data_loader.h>
#include <hso/data_class.h>
#include <filesystem>
#include <memory>
#include <algorithm>

namespace hso {

  std::unique_ptr<Experiment> ExperimentLoader::Load(const std::filesystem::path& experiment_directory){

    std::unique_ptr<Experiment> new_experiment=std::make_unique<Experiment>();

    std::vector<std::filesystem::directory_entry> entries_in_directory(0);

    for (const auto& data_file: std::filesystem::recursive_directory_iterator(experiment_directory)){

      if (std::filesystem::is_regular_file(data_file)){

        entries_in_directory.push_back(data_file);

      }

    }

    std::sort(entries_in_directory.begin(), entries_in_directory.end());

    for (const auto &data_file: entries_in_directory){

      std::string data_id = data_file.path().stem().string();

      std::unique_ptr<Data> data_pointer = DataLoader::Load(data_file.path().string());

      new_experiment->AddDataSet(data_id, std::move(data_pointer));

    }

    return new_experiment;

  }

}
