// MathUtils - Example header-only manual dependency
#pragma once

#include <cmath>
#include <vector>
#include <numeric>

namespace mathutils {
    
    /**
     * Calculate the mean of a vector of values
     */
    template<typename T>
    T mean(const std::vector<T>& values) {
        if (values.empty()) {
            return T{};
        }
        T sum = std::accumulate(values.begin(), values.end(), T{});
        return sum / static_cast<T>(values.size());
    }
    
    /**
     * Calculate the standard deviation of a vector of values
     */
    template<typename T>
    T standard_deviation(const std::vector<T>& values) {
        if (values.size() <= 1) {
            return T{};
        }
        
        T avg = mean(values);
        T sum_sq_diff = T{};
        
        for (const auto& value : values) {
            T diff = value - avg;
            sum_sq_diff += diff * diff;
        }
        
        return std::sqrt(sum_sq_diff / static_cast<T>(values.size() - 1));
    }
    
    /**
     * Linear interpolation between two values
     */
    template<typename T>
    T lerp(T a, T b, T t) {
        return a + t * (b - a);
    }
    
    /**
     * Clamp a value between min and max
     */
    template<typename T>
    T clamp(T value, T min_val, T max_val) {
        return std::max(min_val, std::min(value, max_val));
    }
    
    /**
     * Check if a number is approximately equal to another (for floating point comparison)
     */
    template<typename T>
    bool approximately_equal(T a, T b, T epsilon = static_cast<T>(1e-9)) {
        return std::abs(a - b) < epsilon;
    }
    
} // namespace mathutils