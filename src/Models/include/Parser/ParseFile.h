//
// Created by Alesander Britni on 8/23/23.
//

#ifndef INC_3D_TEST_PARSEOBJECT_H
#define INC_3D_TEST_PARSEOBJECT_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#include "DataStruct/DataInput.h"

#if 1
#include "Logging/Logging.h"
#endif

#if 0
#include <deque>
#include <mutex>
#include <thread>
#endif

namespace s21 {
class ParseFile {
 public:
  using data_type = std::vector<DataInput>;

  data_type Parser(std::string name_file);
  data_type SendVec();
  inline void ClearData() { input_.clear(); }

 private:
  std::vector<DataInput> input_;

  void ReadLine(const std::string& line);
  void ProcessLines();
};
}  // namespace s21

#endif  // INC_3D_TEST_PARSEOBJECT_H
