#include <random>
class RandomGenerator {
public:
  RandomGenerator();
  explicit RandomGenerator(unsigned int seed);

  // Generate uniform random number in [min, max]
  double uniform(double min, double max);

  // Set seed for reproducibility
  void setSeed(unsigned int seed);

private:
  std::mt19937 generator_;
  std::uniform_real_distribution<double> distribution_;
};
