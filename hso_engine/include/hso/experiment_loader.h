#ifndef _EXPERIMENT_LOADER_H_
#define _EXPERIMENT_LOADER_H_

#include <string>
#include <filesystem>
#include <memory>

namespace hso {

  class Experiment;

  class ExperimentLoader {

  public:

    ExperimentLoader() = delete;

    ExperimentLoader(const ExperimentLoader&) = delete;

    ExperimentLoader(const ExperimentLoader&&) = delete;

    ExperimentLoader& operator=(const ExperimentLoader&) = delete;

    ExperimentLoader& operator=(const ExperimentLoader&&) = delete;

    static std::unique_ptr<Experiment> Load(const std::filesystem::path& experiment_directory);

  };

}

#endif //_EXPERIMENT_LOADER_H_
