//
// Created by Alesander Britni on 1/26/24.
//

#include "Random/Random.h"

namespace s21 {

double Random::GetRandomForBios(int size_layer) {
  return ((rand() % 50)) * 0.06 / (size_layer + 15);
}

double Random::GetRandomForWeight(int row) {
  return ((rand() % 100)) * 0.03 / (row + 35.);
}

double Random::GetRandom() {
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_real_distribution<double> uniform_dist(-0.1, 0.1);
  return uniform_dist(e1);
}

}  // namespace s21
