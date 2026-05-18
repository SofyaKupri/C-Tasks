#include "2.hpp"
#include <future>
#include <iostream>
#include <thread>

void producer(std::promise<std::vector<double>> p, int size) {
  try {
    if (size <= 0)
      throw std::runtime_error("Invalid size");
    std::vector<double> data(size, 1.0);
    p.set_value(std::move(data));
  } catch (...) {
    p.set_exception(std::current_exception());
  }
}

void run_part_b() {
  std::cout << "--- Часть B — std::promise / std::future ---" << std::endl;
  std::promise<std::vector<double>> p;
  auto f = p.get_future();

  std::thread t(producer, std::move(p), 0);

  try {
    auto res = f.get();
    std::cout << "size: " << res.size() << std::endl;
  } catch (const std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  t.join();
}