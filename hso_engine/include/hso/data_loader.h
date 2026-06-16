#ifndef _DATA_LOADER_H_
#define _DATA_LOADER_H_

#include <string>
#include <memory>

namespace hso{

  class Data;

  class DataLoader {

  public:

    DataLoader() = delete;

    DataLoader(DataLoader& ) = delete;

    DataLoader(DataLoader&& ) = delete;

    DataLoader& operator=(DataLoader& ) = delete;

    DataLoader& operator=(DataLoader&& ) = delete;

    static std::unique_ptr<Data> Load(std::string file_name_in);

  };

}//hso

#endif //_DATA_LOADER_H_
