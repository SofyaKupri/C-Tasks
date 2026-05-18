#include "2.hpp"
#include <coroutine>
#include <iostream>

template <typename T> struct Generator {
  struct promise_type {
    T val;
    Generator get_return_object() {
      return Generator{
          std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    std::suspend_always yield_value(T v) {
      val = v;
      return {};
    }
    void return_void() {}
  };
  std::coroutine_handle<promise_type> h;
  Generator(std::coroutine_handle<promise_type> handle) : h(handle) {}
  ~Generator() {
    if (h)
      h.destroy();
  }

  struct iter {
    std::coroutine_handle<promise_type> h;
    bool done;
    void operator++() {
      h.resume();
      done = h.done();
    }
    T operator*() { return h.promise().val; }
    bool operator!=(const iter &other) const { return done != other.done; }
  };
  iter begin() {
    h.resume();
    return {h, h.done()};
  }
  iter end() { return {nullptr, true}; }
};

Generator<double> setup_gen(size_t N) {
  for (size_t i = 0; i < N; ++i)
    co_yield static_cast<double>(i);
}

void run_part_c(size_t N) {
  std::cout << "--- Часть C — Корутина C++20 (co_yield)  ---" << std::endl;
  auto gen = setup_gen(N);
  double sum = 0;
  for (auto val : gen) {
    sum += transform_func(val);
  }
  std::cout << "Result: " << sum << std::endl;
}
