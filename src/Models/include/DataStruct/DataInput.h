//
// Created by Slava on 28.11.2023.
//

#ifndef MLP_DATAINPUT_H
#define MLP_DATAINPUT_H

#include <vector>

namespace s21 {
struct DataInput {
  int answer = -1;
  std::vector<double> pixels;

  DataInput() = default;

  void Clear() {
    answer = -1;
    pixels.clear();
  }
};
}  // namespace s21

#endif  // MLP_DATAINPUT_H
