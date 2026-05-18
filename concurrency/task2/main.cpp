#include "2.hpp"
#include <chrono>
#include <iostream>

void benchmark(size_t N) {
  std::cout << "\n===== BENCHMARK N = " << N << " =====" << std::endl;

  auto start = std::chrono::high_resolution_clock::now();
  run_part_a(N);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff_a = end - start;

  start = std::chrono::high_resolution_clock::now();
  run_part_c(N);
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff_c = end - start;

  std::cout << "Futures: " << diff_a.count() << "s" << std::endl;
  std::cout << "Coroutines: " << diff_c.count() << "s" << std::endl;
}

int main() {
  run_part_b();
  benchmark(100000);
  benchmark(1000000);
  return 0;
}