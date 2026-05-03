#ifndef _DATA_LOADER_H_
#define _DATA_LOADER_H_

#include <vector>
#include <string>
#include <initializer_list>
#include <map>
#include <tuple>

namespace hso{

  class Data;

  class DataLoader {

  public:

    static Data Load(std::string file_name_in);

    static void SetBehavior(Data &datastance);


  };

}//hso

#endif //_DATA_LOADER_H_
