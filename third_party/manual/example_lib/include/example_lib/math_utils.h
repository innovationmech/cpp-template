#pragma once

/**
 * Example Manual Dependency Library
 * This demonstrates how to integrate manually managed third-party libraries
 * 
 * Installation:
 * 1. Download or copy library files to third_party/manual/
 * 2. Organize headers in include/ and sources in src/
 * 3. Create CMakeLists.txt for integration
 * 4. Link against the library in your targets
 */

namespace example_lib {

/**
 * Simple mathematical utility functions
 */
class MathUtils {
public:
    /**
     * Calculate the factorial of a number
     * @param n The input number
     * @return The factorial of n
     */
    static long long factorial(int n);
    
    /**
     * Calculate the greatest common divisor of two numbers
     * @param a First number
     * @param b Second number
     * @return The GCD of a and b
     */
    static int gcd(int a, int b);
    
    /**
     * Check if a number is prime
     * @param n The number to check
     * @return True if n is prime, false otherwise
     */
    static bool isPrime(int n);
};

} // namespace example_lib