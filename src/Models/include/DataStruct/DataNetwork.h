//
// Created by Slava on 28.11.2023.
//

#ifndef MLP_DATANETWORK_H
#define MLP_DATANETWORK_H

#include <string>
#include <vector>

namespace s21 {
struct DataNetwork {
  int layers{};
  std::vector<int> size;
  double learning_rate{};
  double moment{};

  DataNetwork() = default;

  DataNetwork(std::vector<int> new_size, double new_lr, double new_m)
      : size(new_size), learning_rate(new_lr), moment(new_m) {
    layers = (int)size.size();
  }
};
}  // namespace s21

#endif  // MLP_DATANETWORK_H
