# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

A command-line C++ application implementing Particle Swarm Optimization (PSO) algorithms for solving global function minimization/maximization problems. This is an educational project focused on understanding PSO algorithms, their implementation, and relative performance.

## Development Commands

The project uses CMake for building and GoogleTest for testing. Build system is not yet configured.

Once configured, typical commands will be:

```bash
# Build the project
cmake -B build -S .
cmake --build build

# Run tests
ctest --test-dir build

# Run the application
./build/pso [options]
```

## Architecture

### Polymorphic Design

The project uses a polymorphic architecture with two primary hierarchies:

1. **Problem Hierarchy**: Base class for optimization problems
   - Base class: `Problem` (or similar)
   - Derived classes for specific test functions (Sphere, Rosenbrock, Rastrigin, Ackley, Griewank, etc.)
   - Each problem defines: objective function, bounds, dimensionality, known global optimum

2. **Swarm/Algorithm Hierarchy**: Base class for PSO variants
   - Base class: `Swarm` (or similar)
   - Derived classes for different swarm intelligence algorithms (Standard PSO, ABC, Cuckoo Search, etc.)
   - Each algorithm implements: initialization, update rules, convergence criteria

### Test Functions

Initial target functions from Wikipedia's optimization test functions:

- **Unimodal**: Sphere, Rosenbrock
- **Multimodal**: Rastrigin, Ackley, Griewank, Schwefel
- See `docs/pso_mathematics_guide.md` sections 6.1-6.3 for mathematical definitions
- See `docs/pso_reference.md` sections 5.1-5.6 for implementation formulas

### Core PSO Algorithm Components

Based on the mathematical framework in the docs:

**Particle representation**:

- Position vector (current solution)
- Velocity vector (rate of change)
- Personal best position
- Personal best fitness

**Update equations**:

```text
v(t+1) = w*v(t) + c1*r1*(pbest - x(t)) + c2*r2*(gbest - x(t))
x(t+1) = x(t) + v(t+1)
```

**Key implementation details**:

- Velocity clamping to prevent explosion
- Boundary handling strategies (absorbing, reflecting, periodic)
- Topology variants (global best, local best, von Neumann)
- Adaptive inertia weight strategies

See `docs/pso_mathematics_guide.md` section 2 for complete mathematical framework.
See `docs/pso_reference.md` for implementation checklist and pseudocode.

## Coding Standards

- **Language**: C++17
- Use C++ STL where appropriate
- Follow modern C++ best practices
- No specific style guide yet defined

### Markdown Standards

When creating or editing markdown files (*.md):

1. **Code Blocks**: Always specify a language for fenced code blocks
   - Use ` ```cpp ` for C++ code
   - Use ` ```bash ` for shell commands
   - Use ` ```text ` for pseudocode, mathematical formulas, or plain text
   - Use ` ```cmake ` for CMake snippets
   - Never use ` ``` ` without a language specifier

2. **Headings**: Always surround headings with blank lines
   - Add a blank line before each heading
   - Add a blank line after each heading

3. **Lists**: Surround lists with blank lines
   - Add a blank line before the first list item
   - Add a blank line after the last list item

4. **No Time Estimates**: Never include time estimates or schedules in planning documents
   - Focus on what needs to be done, not when
   - Let users decide scheduling

5. **Trailing Whitespace**: Avoid trailing spaces at the end of lines

6. **Line Length**: Keep lines reasonably short (under 120 characters when possible) for better readability

## Implementation Strategy

When implementing PSO components:

1. **Start with Standard PSO**: Implement global best (gbest) PSO with sphere function first
2. **Use Strategy Pattern** for:
   - Topology implementations (global, ring, von Neumann)
   - Boundary handling methods
   - Parameter adaptation schemes
   - Initialization strategies
3. **Random Number Generation**: Use `<random>` library with proper seeding
4. **Typical Parameters** (see `docs/pso_reference.md` section 14):
   - Swarm size: 30 particles
   - Inertia weight: 0.7298 (constant) or 0.9→0.4 (linear decreasing)
   - Acceleration coefficients: c1 = c2 = 2.0
   - Velocity limit: v_max = 0.2 × range
   - Max iterations: 1000-5000

## Mathematical Reference

The `docs/` directory contains comprehensive mathematical foundations:

- `pso_mathematics_guide.md`: Detailed mathematical theory, convergence analysis, variants
- `pso_reference.md`: Quick implementation reference, pseudocode, test functions

Refer to these documents when:

- Implementing update equations
- Choosing parameter values
- Understanding algorithm variants
- Adding new test functions
- Implementing performance metrics

## Extension Plan

After basic PSO implementation:

1. Additional test functions (multimodal, fixed-dimension)
2. PSO variants (CLPSO, APSO, QPSO)
3. Different topology structures
4. Other swarm algorithms (Artificial Bee Colony, Cuckoo Search, Firefly Algorithm)
5. Performance analysis and visualization tools
6. Parallel particle evaluation
