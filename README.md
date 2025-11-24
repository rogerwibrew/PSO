# Particle Swarm Optimization (PSO)

A command-line C++ application implementing Particle Swarm Optimization algorithms for solving global function minimization and maximization problems.

## Overview

This is an educational project focused on understanding PSO algorithms, their implementation, and relative performance. The project explores various swarm intelligence algorithms and their application to standard optimization test functions.

## Features

- **Multiple PSO Variants**: Implementation of standard PSO and various algorithmic variants
- **Test Function Suite**: Standard optimization benchmark functions including:
  - Unimodal: Sphere, Rosenbrock
  - Multimodal: Rastrigin, Ackley, Griewank, Schwefel
- **Extensible Architecture**: Polymorphic design supporting multiple swarm algorithms
- **Comprehensive Testing**: GoogleTest-based test suite
- **C++17**: Modern C++ standards and STL usage

## Project Structure

```
PSO/
├── source/          # C++ source files (.cpp)
├── include/         # Header files (.h)
├── tests/           # GoogleTest test files
├── docs/            # Mathematical documentation and references
├── CMakeLists.txt   # CMake build configuration
└── CLAUDE.md        # Development guide for AI assistants
```

## Building the Project

### Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git (for downloading GoogleTest)

### Build Instructions

```bash
# Configure the build
cmake -B build -S .

# Build the project
cmake --build build

# Run the application
./build/pso

# Run tests
ctest --test-dir build --output-on-failure
```

## Architecture

The project uses a polymorphic design with two primary class hierarchies:

### Problem Hierarchy
Base class for optimization problems, with derived classes for specific test functions. Each problem defines:
- Objective function
- Search space bounds
- Dimensionality
- Known global optimum

### Swarm Hierarchy
Base class for PSO variants, with derived classes for different swarm intelligence algorithms. Each algorithm implements:
- Particle initialization
- Velocity and position update rules
- Convergence criteria

## Mathematical Foundation

The core PSO algorithm uses velocity and position update equations:

```
v(t+1) = w*v(t) + c1*r1*(pbest - x(t)) + c2*r2*(gbest - x(t))
x(t+1) = x(t) + v(t+1)
```

Where:
- `w` = inertia weight
- `c1`, `c2` = acceleration coefficients
- `r1`, `r2` = random values in [0,1]
- `pbest` = particle's personal best position
- `gbest` = swarm's global best position

See [`docs/pso_mathematics_guide.md`](docs/pso_mathematics_guide.md) for comprehensive mathematical details and [`docs/pso_reference.md`](docs/pso_reference.md) for implementation reference.

## Development Roadmap

- [x] Project setup with CMake and GoogleTest
- [ ] Base problem class and test function implementations
- [ ] Base swarm class and standard PSO implementation
- [ ] Velocity clamping and boundary handling strategies
- [ ] Performance metrics and logging
- [ ] PSO algorithm variants (CLPSO, APSO, QPSO)
- [ ] Alternative swarm algorithms (ABC, Cuckoo Search, Firefly)
- [ ] Visualization and analysis tools

## Testing

The project uses GoogleTest for unit testing. Tests are automatically discovered and can be run via CTest:

```bash
# Run all tests
ctest --test-dir build

# Run tests with verbose output
ctest --test-dir build --output-on-failure

# Run specific test
./build/pso_tests --gtest_filter=TestName.*
```

## Contributing

This is an educational project. Contributions, suggestions, and discussions about PSO algorithms are welcome.

## License

[To be determined]

## References

- Kennedy, J., & Eberhart, R. (1995). "Particle swarm optimization"
- Clerc, M., & Kennedy, J. (2002). "The particle swarm - explosion, stability, and convergence"
- Shi, Y., & Eberhart, R. (1998). "A modified particle swarm optimizer"
- [Wikipedia: Test functions for optimization](https://en.wikipedia.org/wiki/Test_functions_for_optimization)

## Acknowledgments

Built with modern C++ and GoogleTest framework for educational purposes in understanding swarm intelligence algorithms.
