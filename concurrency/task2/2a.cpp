#include "2.hpp"
#include <future>
#include <iostream>
#include <numeric>
#include <thread>

void run_part_a(size_t N) {
  std::cout << "--- Часть A — std::async + std::future + std::packaged_task ---"
            << std::endl;

  auto load = std::async(std::launch::async, [N]() {
    std::vector<double> data(N);
    std::iota(data.begin(), data.end(), 1.0);
    return data;
  });

  auto transform = std::async(
      std::launch::async,
      [](std::future<std::vector<double>> f) {
        auto data = f.get();
        for (auto &x : data)
          x = transform_func(x);
        return data;
      },
      std::move(load));

  std::packaged_task<double(std::vector<double>)> reduce_task(
      [](std::vector<double> data) {
        return std::accumulate(data.begin(), data.end(), 0.0);
      });

  auto final_future = reduce_task.get_future();
  std::thread t(std::move(reduce_task), transform.get());
  t.join();

  std::cout << "Result: " << final_future.get() << std::endl;
}