# PSO Implementation Plan

## Overview

This plan outlines the implementation of a minimal viable PSO (Particle Swarm Optimization) system with a clean, extensible architecture using strategy patterns.

## Scope

- **Goal**: Working PSO that solves the 2D Sphere function
- **Architecture**: Polymorphic design with strategy patterns
- **Testing**: 2D only initially, comprehensive unit tests
- **Structure**: Header/source file separation

---

## Phase 1: Foundation Types and Utilities

### 1.1 Create Core Type Definitions

**File**: `inlude/pso_types.h`

Define the following structures:

```cpp
struct Particle {
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> personal_best;
    double personal_best_fitness;
    double current_fitness;
};

struct PSOParameters {
    int swarm_size = 30;
    int max_iterations = 1000;
    double inertia_weight = 0.7298;
    double cognitive_coeff = 2.0;
    double social_coeff = 2.0;
    double velocity_clamp_factor = 0.2;
    double fitness_threshold = 1e-6;
    int stagnation_iterations = 100;
};

struct OptimizationResult {
    std::vector<double> best_position;
    double best_fitness;
    int iterations;
    int function_evaluations;
    std::vector<double> fitness_history;
    bool converged;
    std::string convergence_reason;
};
```

**Testing**: Not needed (simple structs)

---

### 1.2 Implement Random Number Generator

**Files**: `include/random_generator.h`, `source/random_generator.cpp`

**Header** (`include/random_generator.h`):

```cpp
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
```

**Implementation Details**:

- Use `std::mt19937` for generation
- Use `std::uniform_real_distribution<double>`
- Default constructor uses `std::random_device` for seed
- `uniform()` method returns value in [min, max]

**Test File**: `tests/random_generator_test.cpp`

- Test: Values are within specified range
- Test: Different seeds produce different sequences
- Test: Same seed produces reproducible results
- Test: Distribution appears uniform (statistical test with 10000 samples)

---

## Phase 2: Problem Hierarchy

### 2.1 Create Problem Base Class

**Files**: `include/problem.h`, `source/problem.cpp`

**Header** (`include/problem.h`):

```cpp
class Problem {
public:
    virtual ~Problem() = default;

    // Evaluate objective function at position x
    virtual double evaluate(const std::vector<double>& x) const = 0;

    // Get problem properties
    virtual int getDimensions() const = 0;
    virtual std::pair<double, double> getBounds() const = 0;
    virtual double getGlobalOptimum() const = 0;
    virtual std::vector<double> getOptimumPosition() const = 0;
    virtual std::string getName() const = 0;

    // Helper: Check if position is within bounds
    bool isWithinBounds(const std::vector<double>& x) const;
};
```

**Implementation** (`source/problem.cpp`):

- Implement `isWithinBounds()` helper method
- Checks each dimension against bounds

**Testing**: Not directly testable (abstract class)

---

### 2.2 Implement Sphere Problem

**Files**: `include/sphere_problem.h`, `source/sphere_problem.cpp`

**Mathematical Formula**:

```text
f(x) = Σ(x[i]²) for i = 1 to n
```

**Properties**:

- Domain: [-5.12, 5.12]^n
- Global minimum: f(0, 0, ..., 0) = 0
- Unimodal (single optimum)
- Smooth, convex

**Header** (`include/sphere_problem.h`):

```cpp
class SphereProblem : public Problem {
public:
    explicit SphereProblem(int dimensions);

    double evaluate(const std::vector<double>& x) const override;
    int getDimensions() const override;
    std::pair<double, double> getBounds() const override;
    double getGlobalOptimum() const override;
    std::vector<double> getOptimumPosition() const override;
    std::string getName() const override;

private:
    int dimensions_;
};
```

**Implementation Details**:

- Constructor validates dimensions > 0
- `evaluate()`: Sum of squared components
- Bounds: [-5.12, 5.12]
- Optimum position: vector of zeros

**Test File**: `tests/problem_test.cpp`

- Test: 2D sphere at (0, 0) returns 0
- Test: 2D sphere at (1, 1) returns 2
- Test: 2D sphere at (3, 4) returns 25
- Test: Dimensions property returns correct value
- Test: Bounds are [-5.12, 5.12]
- Test: Optimum position is all zeros
- Test: Global optimum value is 0
- Test: isWithinBounds works correctly

---

## Phase 3: Strategy Pattern Implementations

### 3.1 Boundary Handler Strategy

**Files**: `include/boundary_handler.h`, `source/boundary_handler.cpp`

**Interface**:

```cpp
class BoundaryHandler {
public:
    virtual ~BoundaryHandler() = default;

    // Handle particle that has moved outside bounds
    virtual void handleBoundary(Particle& particle,
                                const Problem& problem) const = 0;
    virtual std::string getName() const = 0;
};
```

**Implementations**:

1. **AbsorbingBoundary**:
   - If position > max: set position = max, velocity = 0
   - If position < min: set position = min, velocity = 0
   - Prevents further movement into boundary

2. **ReflectingBoundary**:
   - If position > max: set position = max, velocity = -velocity
   - If position < min: set position = min, velocity = -velocity
   - Bounces particle back into search space

3. **PeriodicBoundary**:
   - Wraps position using modulo arithmetic
   - position = min + mod(position - min, max - min)
   - Creates toroidal topology

**Test File**: `tests/boundary_handler_test.cpp`

- Test each strategy with particle outside bounds (above and below)
- Test each strategy leaves in-bounds particles unchanged
- Test velocity behavior for each strategy
- Test multiple dimensions

---

### 3.2 Inertia Weight Strategy

**Files**: `include/inertia_strategy.h`, `source/inertia_strategy.cpp`

**Interface**:

```cpp
class InertiaStrategy {
public:
    virtual ~InertiaStrategy() = default;

    // Get inertia weight for current iteration
    virtual double getWeight(int iteration, int max_iterations) const = 0;
    virtual std::string getName() const = 0;
};
```

**Implementations**:

1. **ConstantInertia**:
   - Returns fixed weight (default 0.7298 - constriction factor)
   - Constructor: `ConstantInertia(double weight = 0.7298)`

2. **LinearDecreasingInertia**:
   - Formula: `w = w_max - (w_max - w_min) * (iteration / max_iterations)`
   - Constructor: `LinearDecreasingInertia(double w_max = 0.9, double w_min = 0.4)`
   - Provides high exploration early, exploitation later

**Test File**: `tests/inertia_strategy_test.cpp`

- Test: Constant returns same value for all iterations
- Test: Linear decreasing starts at w_max, ends at w_min
- Test: Linear decreasing decreases monotonically
- Test: Edge cases (iteration 0, iteration = max)

---

### 3.3 Topology Strategy

**Files**: `include/topology_strategy.h`, `source/topology_strategy.cpp`

**Interface**:

```cpp
class TopologyStrategy {
public:
    virtual ~TopologyStrategy() = default;

    // Get best position from particle's neighborhood
    virtual const std::vector<double>& getBestNeighbor(
        int particle_index,
        const std::vector<Particle>& swarm) const = 0;
    virtual std::string getName() const = 0;
};
```

**Implementation**:

**GlobalTopology** (gbest):

- All particles connected
- Returns global best position from entire swarm
- Fastest convergence, risk of premature convergence

**Test File**: `tests/topology_strategy_test.cpp`

- Test: GlobalTopology returns best particle from swarm
- Test: Works with various swarm sizes
- Test: Handles tie situations (multiple particles with same best fitness)

---

## Phase 4: Standard PSO Implementation

### 4.1 Implement Standard PSO

**Files**: `include/standard_pso.h`, `source/standard_pso.cpp`

**Header**:

```cpp
class StandardPSO {
public:
    StandardPSO(const Problem& problem,
                const PSOParameters& params,
                std::shared_ptr<BoundaryHandler> boundary_handler,
                std::shared_ptr<InertiaStrategy> inertia_strategy,
                std::shared_ptr<TopologyStrategy> topology_strategy);

    // Run optimization
    OptimizationResult optimize();

    // Set random seed for reproducibility
    void setSeed(unsigned int seed);

private:
    void initializeSwarm();
    void updateVelocity(int particle_index, int iteration);
    void updatePosition(int particle_index);
    void evaluateParticle(int particle_index);
    void updatePersonalBest(int particle_index);
    void updateGlobalBest();
    bool checkConvergence(int iteration);
    double calculateDiversity() const;

    const Problem& problem_;
    PSOParameters params_;
    std::vector<Particle> swarm_;
    std::vector<double> global_best_position_;
    double global_best_fitness_;
    int function_evaluations_;
    int stagnation_count_;

    std::shared_ptr<BoundaryHandler> boundary_handler_;
    std::shared_ptr<InertiaStrategy> inertia_strategy_;
    std::shared_ptr<TopologyStrategy> topology_strategy_;
    RandomGenerator random_;
};
```

**Implementation Algorithm**:

1. **initializeSwarm()**:
   - Create swarm_size particles
   - Random positions within problem bounds
   - Random velocities: uniform in [-v_max, v_max] where v_max = velocity_clamp_factor * range
   - Personal best = initial position
   - Evaluate all particles
   - Find global best

2. **updateVelocity()**:

   ```text
   v_new = w * v_old +
           c1 * r1 * (pbest - position) +
           c2 * r2 * (nbest - position)
   ```

   - w from inertia strategy
   - c1 = cognitive_coeff, c2 = social_coeff
   - r1, r2 = random [0, 1]
   - nbest from topology strategy
   - Clamp velocity to [-v_max, v_max]

3. **updatePosition()**:

   ```text
   position_new = position_old + velocity_new
   ```

   - Apply boundary handler

4. **checkConvergence()**:
   - Max iterations reached
   - Fitness below threshold
   - No improvement for stagnation_iterations

5. **optimize()** main loop:

   ```text
   initialize()
   for iteration in 1 to max_iterations:
       for each particle:
           updateVelocity()
           updatePosition()
           evaluateParticle()
           updatePersonalBest()
       updateGlobalBest()
       if checkConvergence():
           break
   return result
   ```

**Test File**: `tests/standard_pso_test.cpp`

- Test: Initialization creates correct number of particles
- Test: Particles initialized within bounds
- Test: Velocities initialized correctly
- Test: Solves 2D sphere function (fitness < 0.01 within 1000 iterations)
- Test: Reproducible with same seed
- Test: Different seeds give different results
- Test: Convergence detection works
- Test: Fitness history is tracked

---

## Phase 5: Application

### 5.1 Update main.cpp

**File**: `source/main.cpp`

**Implementation**:

```cpp
#include <iostream>
#include <iomanip>
#include <memory>
#include "sphere_problem.h"
#include "standard_pso.h"
#include "boundary_handler.h"
#include "inertia_strategy.h"
#include "topology_strategy.h"
#include "pso_types.h"

int main() {
    // Create 2D sphere problem
    SphereProblem problem(2);

    // Set PSO parameters
    PSOParameters params;
    params.swarm_size = 30;
    params.max_iterations = 1000;
    params.cognitive_coeff = 2.0;
    params.social_coeff = 2.0;
    params.velocity_clamp_factor = 0.2;

    // Create strategies
    auto boundary = std::make_shared<AbsorbingBoundary>();
    auto inertia = std::make_shared<LinearDecreasingInertia>(0.9, 0.4);
    auto topology = std::make_shared<GlobalTopology>();

    // Create and run PSO
    StandardPSO pso(problem, params, boundary, inertia, topology);

    std::cout << "Particle Swarm Optimization\n";
    std::cout << "===========================\n\n";
    std::cout << "Problem: " << problem.getName() << "\n";
    std::cout << "Dimensions: " << problem.getDimensions() << "\n";
    std::cout << "Swarm size: " << params.swarm_size << "\n";
    std::cout << "Max iterations: " << params.max_iterations << "\n\n";

    std::cout << "Running optimization...\n\n";

    OptimizationResult result = pso.optimize();

    // Display results
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Optimization Complete!\n";
    std::cout << "=====================\n\n";
    std::cout << "Best fitness: " << result.best_fitness << "\n";
    std::cout << "Best position: [";
    for (size_t i = 0; i < result.best_position.size(); ++i) {
        std::cout << result.best_position[i];
        if (i < result.best_position.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "Iterations: " << result.iterations << "\n";
    std::cout << "Function evaluations: " << result.function_evaluations << "\n";
    std::cout << "Converged: " << (result.converged ? "Yes" : "No") << "\n";
    std::cout << "Reason: " << result.convergence_reason << "\n\n";

    std::cout << "Known optimum: " << problem.getGlobalOptimum() << "\n";
    std::cout << "Error: " << std::abs(result.best_fitness - problem.getGlobalOptimum()) << "\n";

    return 0;
}
```

**Testing**: Manual - run `./build/pso` and verify output

---

## Phase 6: Testing and Validation

### 6.1 Run All Tests

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

**Expected**: All tests pass

### 6.2 Run Application

```bash
./build/pso
```

**Expected Output**:

- Optimization completes successfully
- Best fitness very close to 0 (< 0.01)
- Best position very close to [0, 0]
- Converged = Yes

### 6.3 Validate Results

- Run multiple times, should consistently find optimum
- Error from known optimum should be < 0.01
- Should converge in < 1000 iterations for 2D sphere

---

## Implementation Checklist

### Phase 1: Foundation

- [ ] Create `include/pso_types.h` with Particle, PSOParameters, OptimizationResult
- [ ] Create `include/random_generator.h` and `source/random_generator.cpp`
- [ ] Create `tests/random_generator_test.cpp` and verify all tests pass

### Phase 2: Problem Hierarchy

- [ ] Create `include/problem.h` and `source/problem.cpp` (base class)
- [ ] Create `include/sphere_problem.h` and `source/sphere_problem.cpp`
- [ ] Create `tests/problem_test.cpp` and verify all tests pass

### Phase 3: Strategies

- [ ] Create `include/boundary_handler.h` and `source/boundary_handler.cpp`
  - [ ] Implement AbsorbingBoundary
  - [ ] Implement ReflectingBoundary
  - [ ] Implement PeriodicBoundary
- [ ] Create `tests/boundary_handler_test.cpp` and verify all tests pass
- [ ] Create `include/inertia_strategy.h` and `source/inertia_strategy.cpp`
  - [ ] Implement ConstantInertia
  - [ ] Implement LinearDecreasingInertia
- [ ] Create `tests/inertia_strategy_test.cpp` and verify all tests pass
- [ ] Create `include/topology_strategy.h` and `source/topology_strategy.cpp`
  - [ ] Implement GlobalTopology
- [ ] Create `tests/topology_strategy_test.cpp` and verify all tests pass

### Phase 4: PSO Implementation

- [ ] Create `include/standard_pso.h` and `source/standard_pso.cpp`
  - [ ] Implement constructor and initialization
  - [ ] Implement velocity update
  - [ ] Implement position update
  - [ ] Implement fitness evaluation
  - [ ] Implement best updates
  - [ ] Implement convergence checking
  - [ ] Implement main optimize() loop
- [ ] Create `tests/standard_pso_test.cpp` and verify all tests pass

### Phase 5: Application

- [ ] Update `source/main.cpp` with PSO demonstration
- [ ] Build and run application
- [ ] Verify results are correct

### Phase 6: Final Validation

- [ ] All unit tests pass
- [ ] Application runs without errors
- [ ] PSO finds optimum within tolerance
- [ ] Results are reproducible with same seed
- [ ] Update CMakeLists.txt if needed
- [ ] Commit changes to git

---

## CMakeLists.txt Updates Needed

The existing CMakeLists.txt should work, but verify:

- All source files in `source/` are compiled
- All header files in `include/` are accessible
- All test files in `tests/` are compiled and linked with GTest
- C++17 is enabled

If needed, ensure:

```cmake
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES "source/*.cpp")
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
```

---

## Success Criteria

The MVP is complete when:

1. ✅ All unit tests pass
2. ✅ Application compiles without warnings
3. ✅ PSO solves 2D sphere function with error < 0.01
4. ✅ Code follows C++17 standards
5. ✅ Clean separation: headers in `include/`, implementation in `source/`, tests in `tests/`
6. ✅ Strategy pattern allows easy extension

---

## Future Extensions (Not in MVP)

After MVP is complete, consider:

- Additional test functions (Rosenbrock, Rastrigin, Ackley, Griewank, Schwefel)
- Ring and Von Neumann topologies
- Additional inertia strategies (adaptive)
- Command-line argument parsing
- Higher dimensions (5D, 10D, 30D)
- Performance metrics and logging
- Visualization output (CSV for plotting)
- Other swarm algorithms (ABC, Cuckoo Search)
- Parallel particle evaluation

---

## Mathematical Reference

**PSO Update Equations**:

```text
v[i](t+1) = w * v[i](t) + c1 * r1 * (pbest[i] - x[i](t)) + c2 * r2 * (gbest - x[i](t))
x[i](t+1) = x[i](t) + v[i](t+1)
```

**Velocity Clamping**:

```text
v_max = velocity_clamp_factor * (x_max - x_min)
v = max(-v_max, min(v_max, v))
```

**Sphere Function**:

```text
f(x) = Σ x[i]² for i = 0 to n-1
```

**Standard Parameters**:

- Swarm size: 30
- c1 (cognitive): 2.0
- c2 (social): 2.0
- w (inertia): 0.7298 or 0.9→0.4
- v_max factor: 0.2

**Constants**:

```cpp
const double PI = 3.14159265358979323846;
const double E = 2.71828182845904523536;
```

---

## Tips and Best Practices

1. **Test as you go**: Don't implement everything then test. Write tests for each component.

2. **Use const correctness**: Mark methods that don't modify state as `const`.

3. **Use references**: Pass `std::vector` by const reference to avoid copies.

4. **RAII**: Use smart pointers (`std::shared_ptr`, `std::unique_ptr`) for strategies.

5. **Validate inputs**: Check dimensions > 0, parameters > 0, etc.

6. **Clear error messages**: Use descriptive exceptions or assertions.

7. **Document as you go**: Add comments for non-obvious algorithms.

8. **Keep it simple**: Don't over-engineer. Get it working first.

---

## Time Estimates

Avoid providing time estimates in planning documents as per project guidelines.
