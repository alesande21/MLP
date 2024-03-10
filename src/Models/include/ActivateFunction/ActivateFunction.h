//
// Created by Alesander Britni on 11/20/23.
//

#ifndef MLP_ACTIVATEFUNCTION_H
#define MLP_ACTIVATEFUNCTION_H

#include <cmath>
#include <vector>

enum ActivateFunc { sigmoid = 1, ReLU, thx };

namespace s21 {
class ActivateFunction {
 public:
  ActivateFunction() = default;
  void Set();
  void Use(std::vector<double> &value, int n);
  double UseDer(double value);

 private:
  ActivateFunc act_func_;
};
}  // namespace s21

#endif  // MLP_ACTIVATEFUNCTION_H
