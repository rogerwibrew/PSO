#include <gtest/gtest.h>
#include "random_generator.h"

// Test fixture for RandomGenerator tests
class RandomGeneratorTest : public ::testing::Test {
protected:
  // You can add common setup here if needed
  void SetUp() override {
    // Called before each test
  }

  void TearDown() override {
    // Called after each test
  }
};

// Test 1: Reproducibility with same seed
// This demonstrates that the same seed produces the same sequence of numbers
TEST_F(RandomGeneratorTest, SameSeedProducesSameSequence) {
  // Create two generators with the same seed
  RandomGenerator rng1(12345);
  RandomGenerator rng2(12345);

  // Generate 10 random numbers from each
  for (int i = 0; i < 10; ++i) {
    double val1 = rng1.uniform(0.0, 1.0);
    double val2 = rng2.uniform(0.0, 1.0);

    // They should be exactly equal
    EXPECT_DOUBLE_EQ(val1, val2)
        << "Failed at iteration " << i
        << ": rng1=" << val1 << ", rng2=" << val2;
  }
}

// Test 2: Different seeds produce different sequences
TEST_F(RandomGeneratorTest, DifferentSeedsProduceDifferentSequences) {
  RandomGenerator rng1(12345);
  RandomGenerator rng2(54321);

  // Generate 10 numbers from each
  bool foundDifference = false;
  for (int i = 0; i < 10; ++i) {
    double val1 = rng1.uniform(0.0, 1.0);
    double val2 = rng2.uniform(0.0, 1.0);

    if (val1 != val2) {
      foundDifference = true;
      break;
    }
  }

  // Different seeds should produce different sequences
  EXPECT_TRUE(foundDifference)
      << "Different seeds should produce different random sequences";
}

// Test 3: Generated values stay within specified bounds
TEST_F(RandomGeneratorTest, UniformStaysWithinBounds) {
  RandomGenerator rng(42);

  // Test with positive range
  for (int i = 0; i < 1000; ++i) {
    double val = rng.uniform(5.0, 10.0);
    EXPECT_GE(val, 5.0) << "Value " << val << " is below lower bound";
    EXPECT_LE(val, 10.0) << "Value " << val << " is above upper bound";
  }

  // Test with negative range
  for (int i = 0; i < 1000; ++i) {
    double val = rng.uniform(-10.0, -5.0);
    EXPECT_GE(val, -10.0) << "Value " << val << " is below lower bound";
    EXPECT_LE(val, -5.0) << "Value " << val << " is above upper bound";
  }

  // Test with range crossing zero
  for (int i = 0; i < 1000; ++i) {
    double val = rng.uniform(-5.0, 5.0);
    EXPECT_GE(val, -5.0) << "Value " << val << " is below lower bound";
    EXPECT_LE(val, 5.0) << "Value " << val << " is above upper bound";
  }
}

// Test 4: setSeed() resets the generator to produce the same sequence
TEST_F(RandomGeneratorTest, SetSeedResetsGenerator) {
  RandomGenerator rng(12345);

  // Generate first sequence
  std::vector<double> sequence1;
  for (int i = 0; i < 10; ++i) {
    sequence1.push_back(rng.uniform(0.0, 1.0));
  }

  // Reset with the same seed
  rng.setSeed(12345);

  // Generate second sequence
  std::vector<double> sequence2;
  for (int i = 0; i < 10; ++i) {
    sequence2.push_back(rng.uniform(0.0, 1.0));
  }

  // Both sequences should be identical
  ASSERT_EQ(sequence1.size(), sequence2.size());
  for (size_t i = 0; i < sequence1.size(); ++i) {
    EXPECT_DOUBLE_EQ(sequence1[i], sequence2[i])
        << "Sequences differ at index " << i;
  }
}

// Test 5: Default constructor produces different sequences on different instances
TEST_F(RandomGeneratorTest, DefaultConstructorProducesDifferentSequences) {
  RandomGenerator rng1;
  RandomGenerator rng2;

  // Generate numbers from both
  bool foundDifference = false;
  for (int i = 0; i < 10; ++i) {
    double val1 = rng1.uniform(0.0, 1.0);
    double val2 = rng2.uniform(0.0, 1.0);

    if (val1 != val2) {
      foundDifference = true;
      break;
    }
  }

  // Default constructors should produce different sequences
  // (though there's a tiny chance they might be the same)
  EXPECT_TRUE(foundDifference)
      << "Default constructors should typically produce different sequences";
}

// Test 6: Uniform works correctly with negative ranges
TEST_F(RandomGeneratorTest, UniformWithNegativeRange) {
  RandomGenerator rng(777);

  for (int i = 0; i < 100; ++i) {
    double val = rng.uniform(-100.0, -50.0);
    EXPECT_GE(val, -100.0);
    EXPECT_LE(val, -50.0);
  }
}

// Test 7: Uniform works correctly with large ranges
TEST_F(RandomGeneratorTest, UniformWithLargeRange) {
  RandomGenerator rng(999);

  for (int i = 0; i < 100; ++i) {
    double val = rng.uniform(-1000.0, 1000.0);
    EXPECT_GE(val, -1000.0);
    EXPECT_LE(val, 1000.0);
  }
}

// Test 8: Uniform works with very small ranges
TEST_F(RandomGeneratorTest, UniformWithSmallRange) {
  RandomGenerator rng(123);

  for (int i = 0; i < 100; ++i) {
    double val = rng.uniform(0.0, 0.001);
    EXPECT_GE(val, 0.0);
    EXPECT_LE(val, 0.001);
  }
}

// Test 9: Basic distribution check (values should be somewhat evenly distributed)
TEST_F(RandomGeneratorTest, BasicDistributionCheck) {
  RandomGenerator rng(456);
  const int numSamples = 10000;
  const double min = 0.0;
  const double max = 10.0;
  const double midpoint = (min + max) / 2.0;

  int countLower = 0;
  int countUpper = 0;

  for (int i = 0; i < numSamples; ++i) {
    double val = rng.uniform(min, max);
    if (val < midpoint) {
      countLower++;
    } else {
      countUpper++;
    }
  }

  // With a uniform distribution, we expect roughly 50/50 split
  // Allow for some statistical variation (40% to 60%)
  double lowerRatio = static_cast<double>(countLower) / numSamples;
  EXPECT_GT(lowerRatio, 0.40) << "Lower half has too few samples";
  EXPECT_LT(lowerRatio, 0.60) << "Lower half has too many samples";
}
