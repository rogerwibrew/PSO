# RandomGenerator Implementation Guide

## Overview

This guide provides implementation advice for the `RandomGenerator` class in `source/random_generator.cpp`. The RandomGenerator is a critical component for PSO algorithms, providing high-quality random numbers for particle initialization and velocity updates.

## Core Design Recommendations

### 1. Use Modern C++ `<random>` Library

**Don't use**: `rand()` or `srand()` - these are outdated and produce lower quality randomness.

**Do use**: The `<random>` library with:

- `std::mt19937` or `std::mt19937_64` as your random engine
  - Mersenne Twister algorithm
  - Industry standard
  - High-quality pseudo-random number generation
  - Long period (2^19937 - 1)
- `std::uniform_real_distribution<double>` for generating uniform random numbers in a range

### 2. Required Member Variables

```cpp
private:
    std::mt19937 engine;  // Random number engine
    // Optional: std::uniform_real_distribution<double> distribution;
```

### 3. Seeding Strategy

**Default Constructor** (non-deterministic):

- Use `std::random_device` to get a non-deterministic seed
- Be aware: on some systems `std::random_device` might be deterministic
- Consider combining `std::random_device` with `std::chrono::high_resolution_clock::now()` for extra entropy
- This is useful for production runs where you want different results each time

**Example initialization in member initializer list**:

```cpp
// Simple approach
RandomGenerator::RandomGenerator()
    : generator_(std::random_device{}()) {
}

// More robust (combines random_device with time)
RandomGenerator::RandomGenerator()
    : generator_(std::random_device{}() ^
                 std::chrono::high_resolution_clock::now().time_since_epoch().count()) {
}
```

**Note on `std::random_device{}()` syntax**:

- `std::random_device{}` - Creates a temporary `random_device` object using brace initialization
- `()` - Calls the temporary object's `operator()` to generate a random seed value
- This is equivalent to the more verbose form:
  ```cpp
  std::random_device rd;
  generator_(rd())
  ```
- The compact form `std::random_device{}()` is idiomatic in member initializer lists

**Seeded Constructor** (deterministic):

- Directly initialize the engine with the provided seed
- Critical for reproducibility in PSO experiments
- Allows you to reproduce exact runs for debugging and comparison
- Same seed = same sequence of random numbers

### 4. Implementation Patterns for `uniform()`

**Approach 1: Create Distribution On-Demand** (simpler):

```cpp
std::uniform_real_distribution<double> dist(min, max);
return dist(engine);
```

Pros: Simple, straightforward
Cons: Small overhead from creating distribution object each call

**Approach 2: Reuse Distribution** (more efficient):

```cpp
// Store distribution as member variable
distribution.param(std::uniform_real_distribution<double>::param_type(min, max));
return distribution(engine);
```

Pros: More efficient for repeated calls (PSO calls this thousands of times)
Cons: Slightly more complex

**Recommendation**: For PSO use cases, Approach 2 is preferable due to the high frequency of random number generation.

### 5. Thread Safety Consideration

**Initial Implementation**:

- Basic implementation doesn't need thread safety
- Each `RandomGenerator` instance is independent

**Future Considerations**:

If you parallelize particle evaluation later, consider:

- Give each thread/particle its own `RandomGenerator` instance (recommended)
- OR implement synchronization for shared generator (more complex)

### 6. PSO-Specific Considerations

In PSO algorithms, you'll be generating random numbers for:

1. **Initial positions**: `uniform(lower_bound, upper_bound)` for each dimension
2. **Initial velocities**: Often `uniform(-v_max, v_max)` or a fraction of position range
3. **Velocity updates**: Random values r1, r2 in `[0, 1]` for the update equation

**Optional Convenience Methods**:

Consider adding helper methods for common cases:

```cpp
// Generate random number in [0, 1]
double uniform01();

// Generate random number in [-1, 1]
double uniformSymmetric();
```

These can make PSO algorithm code cleaner and more readable.

### 7. Testing Approach

When implementing and testing `RandomGenerator`:

**Test reproducibility**:

```cpp
RandomGenerator rng1(12345);
RandomGenerator rng2(12345);
// Both should generate identical sequences
```

**Test bounds**:

```cpp
// Generate many samples and verify all are within [min, max]
for (int i = 0; i < 10000; ++i) {
    double val = rng.uniform(0.0, 1.0);
    assert(val >= 0.0 && val <= 1.0);
}
```

**Test distribution quality** (optional):

- MT19937 is well-validated, so exhaustive testing isn't necessary
- Can do basic statistical tests (mean, variance) if desired
- More important to test your PSO algorithms using the generator

## Example Usage Pattern

```cpp
// For reproducible experiments
RandomGenerator rng(42);  // Fixed seed

// Initialize particle positions
for (int i = 0; i < swarm_size; ++i) {
    for (int d = 0; d < dimensions; ++d) {
        particles[i].position[d] = rng.uniform(lower_bound, upper_bound);
    }
}

// In velocity update
double r1 = rng.uniform(0.0, 1.0);
double r2 = rng.uniform(0.0, 1.0);
```

## Additional Resources

- C++ `<random>` library documentation
- `docs/pso_mathematics_guide.md` - Section 2 for PSO update equations
- `docs/pso_reference.md` - Section 14 for typical PSO parameters

## Implementation Checklist

- [ ] Include `<random>` header
- [ ] Add `std::mt19937` engine as member variable
- [ ] Implement default constructor with non-deterministic seeding
- [ ] Implement seeded constructor for reproducibility
- [ ] Implement `uniform(min, max)` method
- [ ] Implement `setSeed()` method
- [ ] Add tests for reproducibility
- [ ] Add tests for bound checking
- [ ] Consider adding convenience methods (`uniform01()`, etc.)
