//
// Created by Alesander Britni on 1/26/24.
//

#ifndef MLP_CLOCKS_H
#define MLP_CLOCKS_H

#include <chrono>

namespace s21 {
class Clocks {
 public:
  using time_type = std::chrono::time_point<std::chrono::steady_clock>;

  static inline unsigned long long Rdtsc() {
    unsigned hi, lo;
    __asm__ __volatile__(
        "xor %%eax, %%eax    \n"
        "xor %%edx, %%edx    \n"
        "rdtsc \n"
        : "=a"(lo), "=d"(hi));
    return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
  }

  static time_type StdTimer();
};
}  // namespace s21

#endif  // MLP_CLOCKS_H
