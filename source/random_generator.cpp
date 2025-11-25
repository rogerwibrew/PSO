#include "random_generator.h"
#include <chrono>

// Default constructor
RandomGenerator::RandomGenerator()
    : generator_(std::random_device{}() ^
                 std::chrono::high_resolution_clock::now()
                     .time_since_epoch()
                     .count()) {}

// Constructor with explicit seed
RandomGenerator::RandomGenerator(unsigned int seed) : generator_(seed) {}

// Generate uniform random number in [min, max]
double RandomGenerator::uniform(double min, double max) {
  distribution_.param(
      std::uniform_real_distribution<double>::param_type(min, max));
  return distribution_(generator_);
}

// Set seed for reproducibility
void RandomGenerator::setSeed(unsigned int seed) { generator_.seed(seed); }
