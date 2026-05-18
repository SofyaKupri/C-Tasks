#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct LogEntry {
  long long timestamp;
  int user_id;
  std::string event_type;
  int value;
};

struct AggregateResult {
  long long count = 0;
  long long total_value = 0;
  std::set<int> unique_users;
};

std::mutex global_mutex;
AggregateResult final_result;
std::atomic<bool> is_finished(false);

void process_chunk(const std::vector<LogEntry> &all_data, size_t start, size_t end) {
  long long local_count = 0;
  long long local_sum = 0;
  std::set<int> local_users;

  for (size_t i = start; i < end; ++i) {
    const auto &entry = all_data[i];
    local_count++;
    local_sum += entry.value;
    local_users.insert(entry.user_id);
  }

  std::lock_guard<std::mutex> lock(global_mutex);
  final_result.count += local_count;
  final_result.total_value += local_sum;
  final_result.unique_users.insert(local_users.begin(), local_users.end());
}

void progress_indicator() {
  while (!is_finished.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    if (!is_finished.load()) {
      std::cout << "[PROGRESS] Processing logs..." << std::endl;
    }
  }
  std::cout << "[PROGRESS] Done!" << std::endl;
}

void run_test(size_t N, int num_threads) {
  {
    std::lock_guard<std::mutex> lock(global_mutex);
    final_result.count = 0;
    final_result.total_value = 0;
    final_result.unique_users.clear();
  }
  is_finished = false;
  std::cout << "\n>>> Testing with N = " << N << " lines and " << num_threads
            << " threads" << std::endl;

  std::vector<LogEntry> all_entries;
  all_entries.reserve(N);
  for (int i = 0; i < N; ++i) {
    all_entries.push_back({1714983812345, rand() % 1000, "CLICK", rand() % 10});
  }

  auto start_t1 = std::chrono::high_resolution_clock::now();
  AggregateResult t1_res;
  for (auto &e : all_entries) {
    t1_res.count++;
    t1_res.total_value += e.value;
    t1_res.unique_users.insert(e.user_id);
  }
  auto end_t1 = std::chrono::high_resolution_clock::now();

  auto start_tn = std::chrono::high_resolution_clock::now();

  std::thread indicator(progress_indicator);
  indicator.detach();

  size_t total_size = all_entries.size();
  size_t chunk_size = total_size / num_threads;
  std::vector<std::thread> workers;
  for (int i = 0; i < num_threads; ++i) {
    size_t start = i * chunk_size;
    size_t end = (i == num_threads - 1) ? total_size : start + chunk_size;
    workers.emplace_back(process_chunk, std::cref(all_entries), start, end);
  }

  for (auto &t : workers) {
    t.join();
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  is_finished = true;

  auto end_tn = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> dur1 = end_t1 - start_t1;
  std::chrono::duration<double> durn = end_tn - start_tn;

  std::cout << "\n--- Statistics ---" << std::endl;
  std::cout << "Total events: " << final_result.count << std::endl;
  std::cout << "Unique users: " << final_result.unique_users.size()
            << std::endl;
  std::cout << "Time (Single-thread): " << dur1.count() << "s" << std::endl;
  std::cout << "Time (Multi-thread): " << durn.count() << "s" << std::endl;
  std::cout << "Speedup: " << dur1.count() / durn.count() << "x" << std::endl;
}

int main(int argc, char *argv[]) {
  int num_threads = std::stoi(argv[1]);
  run_test(1e4, num_threads);
  run_test(1e5, num_threads);
  run_test(1e6, num_threads);
  return 0;
}
