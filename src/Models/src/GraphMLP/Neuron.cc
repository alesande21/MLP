//
// Created by Alesander Britni on 1/25/24.
//

#include "GraphMLP/Neuron.h"

namespace s21 {

void Neuron::ResizeWeight(int col) { weight_.resize(col); }

void Neuron::ResizeWeightError(int col) { weight_err_.resize(col); }

void Neuron::SetRandom([[maybe_unused]] int row) {
  for (int i = 0; i < (int)weight_.size(); ++i) {
#if 1
    weight_[i] = Random::GetRandomForWeight(row);
#endif
  }
}

void Neuron::SetRandomBios([[maybe_unused]] int col) {
#if 1

  bios_ = Random::GetRandomForBios(col);

#endif
}

void Neuron::MultiplyByWeight(std::vector<Neuron> &prev_layer) {
  if (prev_layer.size() != weight_.size()) {
    throw std::invalid_argument("prev_layer.size() != weight_.size())");
  }
  double temp_value{};
  for (int i = 0; i < (int)weight_.size(); ++i) {
    temp_value += weight_[i] * prev_layer[i].value_;
  }
  value_ = temp_value;
}

void Neuron::SumWithBios() { value_ += bios_; }

void Neuron::CalculateError(double correct_value) {
  error_ = (correct_value - value_) * value_ * (1 - value_);
}

void Neuron::ActivateFunc(ActivateFunction &act_func) {
  double val = value_;
  value_ = act_func.UseDer(value_);
  if (val == value_) {
    std::cout << "ERRRRRRRRROR";
    return;
  }
}

void Neuron::UpdateWeight(double lr, double moment,
                          [[maybe_unused]] double prev_value,
                          std::vector<Neuron> &prev_neurons) {
  double tmp{};
  for (int i = 0; i < (int)weight_.size(); ++i) {
    weight_err_[i] *= moment;
    tmp = prev_neurons[i].value_ * error_ * lr + weight_err_[i];
    weight_[i] += tmp;
    weight_err_[i] = tmp;
  }
}

void Neuron::BiosUpdate(double lr) { bios_ += lr * error_; }

void Neuron::Print() {
  for (int i = 0; i < (int)weight_.size(); ++i) {
    std::cout << weight_[i] << " ";
  }
}

void Neuron::SaveWeight(std::ofstream &out) {
  for (int j = 0; j < (int)weight_.size(); ++j) {
    double val = weight_[j];
    out << val << " ";
  }
}

void Neuron::LoadWeight(std::string &line) {
  std::istringstream iss(line);
  int j = 0;
  double val{};
  while (iss >> val && j < (int)weight_.size()) {
    weight_[j] = val;
    ++j;
  }
}

double &Neuron::GetValue() { return value_; }

double &Neuron::GetError() { return error_; }

double &Neuron::GetValueWeight(int index) { return weight_[index]; }

double &Neuron::GetBios() { return bios_; }

}  // namespace s21
