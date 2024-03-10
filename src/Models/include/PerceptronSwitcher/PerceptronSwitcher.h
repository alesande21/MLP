//
// Created by Alesander Britni on 1/15/24.
//

#ifndef MLP_PERCEPTRONSWITCHER_H
#define MLP_PERCEPTRONSWITCHER_H

#include "GraphMLP/GraphMLP.h"
#include "MatrixMLP/MatrixMLP.h"

namespace s21 {
class PerceptronSwitcher {
 public:
  PerceptronSwitcher();
  BaseMLP *Switcher(int num);
  ~PerceptronSwitcher();

 private:
  MatrixMLP *matrix_;
  GraphMLP *graph_;
};
}  // namespace s21

#endif  // MLP_PERCEPTRONSWITCHER_H
