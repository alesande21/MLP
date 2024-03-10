//
// Created by Alesander Britni on 1/15/24.
//

#include "PerceptronSwitcher/PerceptronSwitcher.h"

namespace s21 {

PerceptronSwitcher::PerceptronSwitcher() {
  graph_ = new GraphMLP();
  matrix_ = new MatrixMLP();
}

BaseMLP *PerceptronSwitcher::Switcher(int num) {
  if (num == 0) {
    return matrix_;
  }
  return graph_;
}

PerceptronSwitcher::~PerceptronSwitcher() {
  delete graph_;
  delete matrix_;
}

}  // namespace s21
