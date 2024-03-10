//
// Created by Alesander Britni on 1/26/24.
//

#ifndef MLP_RANDOM_H
#define MLP_RANDOM_H

#include <random>

namespace s21 {
class Random {
 public:
  static double GetRandomForBios(int size_layer);
  static double GetRandomForWeight(int row);
  static double GetRandom();
};
}  // namespace s21

#endif  // MLP_RANDOM_H
