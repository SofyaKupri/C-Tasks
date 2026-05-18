#pragma once
#include <vector>
#include <cmath>

inline double transform_func(double x) {
    return std::sqrt(std::abs(x)) * std::log1p(std::abs(x));
}

void run_part_a(size_t N);
void run_part_b();
void run_part_c(size_t N);