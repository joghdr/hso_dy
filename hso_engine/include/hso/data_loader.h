#ifndef _DATA_LOADER_H_
#define _DATA_LOADER_H_

#include <string>

namespace hso{

  class Data;

  class DataLoader {

    DataLoader() = delete;

  public:

    static Data Load(std::string file_name_in);

  };

}//hso

#endif //_DATA_LOADER_H_
