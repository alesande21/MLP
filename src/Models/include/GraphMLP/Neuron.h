//
// Created by Alesander Britni on 1/25/24.
//

#ifndef MLP_NEURON_H
#define MLP_NEURON_H

#include <iostream>
//#include <err.h>

#include <fstream>
#include <random>
#include <sstream>
#include <vector>

#include "ActivateFunction/ActivateFunction.h"
#include "Random/Random.h"

namespace s21 {
class Neuron {
 public:
  Neuron() = default;

  void ResizeWeight(int col);
  void ResizeWeightError(int col);
  void SetRandom(int row);
  void SetRandomBios(int col);
  void MultiplyByWeight(std::vector<Neuron> &prev_layer);
  void SumWithBios();
  void CalculateError(double correct_value);
  void ActivateFunc(ActivateFunction &act_func);
  void UpdateWeight(double lr, double moment, double prev_value,
                    std::vector<Neuron> &prev_neurons);
  void BiosUpdate(double lr);
  void Print();
  void SaveWeight(std::ofstream &out);
  void LoadWeight(std::string &line);
  double &GetValue();
  double &GetError();
  double &GetValueWeight(int index);
  double &GetBios();

 private:
  double value_{};
  std::vector<double> weight_{};
  std::vector<double> weight_err_{};
  double bios_{};
  double error_{};
};
}  // namespace s21

#endif  // MLP_NEURON_H
