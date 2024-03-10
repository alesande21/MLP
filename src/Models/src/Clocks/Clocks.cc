//
// Created by Alesander Britni on 1/26/24.
//

#include "Clocks/Clocks.h"

namespace s21 {
Clocks::time_type Clocks::StdTimer() {
  return std::chrono::steady_clock::now();
}
}  // namespace s21
