//
// Created by Alesander Britni on 1/25/24.
//

#ifndef MLP_LAYER_H
#define MLP_LAYER_H

#include <fstream>
#include <sstream>
#include <vector>

#include "ActivateFunction/ActivateFunction.h"
#include "Neuron.h"

namespace s21 {

class Layer {
 public:
  Layer() = default;
  void SetSizeNeurons(int row, int col);
  void RecalculateNeurons(std::vector<Neuron> &prev_layer,
                          ActivateFunction &act_func);
  int SearchMaxIndex();
  void SetInput(std::vector<double> &values);
  void BPForLastLayer(double expect);
  void BPForOtherLayers(std::vector<Neuron> &next_layer);
  void WeightUpdate(double lr, double moment, std::vector<Neuron> &prev_layer);
  void BiosUpdate(double lr);
  void SaveWeight(std::ofstream &out);
  void LoadWeight(std::ifstream &file);
  void PrintLastNeurons();
  void Print(int l);
  int Size();
  std::vector<Neuron> &GetNeurons();

 private:
  int size_{};
  std::vector<Neuron> neurons_;
};
}  // namespace s21

#endif  // MLP_LAYER_H
