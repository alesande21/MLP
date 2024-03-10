//
// Created by Alesander Britni on 11/20/23.
//
#include "ActivateFunction/ActivateFunction.h"

#include <iostream>

namespace s21 {

void ActivateFunction::Set() {
#if 1
  act_func_ = sigmoid;
#elif 0
  act_func_ = ReLU;
#endif
}

void ActivateFunction::Use(std::vector<double> &value, int n) {
  if (act_func_ == ActivateFunc::sigmoid) {
    for (int i = 0; i < n; ++i) {
      value[i] = 1 / (1 + exp(-value[i]));
    }
  } else if (act_func_ == ActivateFunc::ReLU) {
    for (int i = 0; i < n; ++i) {
      if (value[i] < 0)
        value[i] *= 0.01;
      else if (value[i] > 1)
        value[i] = 1. + 0.01 * (value[i] - 1.);
    }
  } else {
    throw std::runtime_error("Error actFunc \n");
  }
}

double ActivateFunction::UseDer(double value) {
  if (act_func_ == ActivateFunc::sigmoid) {
    value = 1 / (1 + exp(-value));
  } else if (act_func_ == ActivateFunc::ReLU) {
    if (value < 0 || value > 1) value = 0.01;
  } else {
    throw std::runtime_error("Error actFunc \n");
  }

  return value;
}

}  // namespace s21
