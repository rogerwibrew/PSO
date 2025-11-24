# Particle Swarm Optimization: Mathematical Foundations and Algorithms

## 1. Introduction and Motivation

Particle Swarm Optimization (PSO) is a population-based stochastic optimization technique inspired by the social behavior of bird flocking and fish schooling. Developed by Kennedy and Eberhart (1995), PSO searches for optima by having particles move through the solution space based on their own experience and that of their neighbors.

## 2. Mathematical Framework

### 2.1 Problem Definition

Given an objective function f: ℝⁿ → ℝ, find:
- **Minimization**: x* = argmin f(x), x ∈ S ⊆ ℝⁿ
- **Maximization**: x* = argmax f(x), x ∈ S ⊆ ℝⁿ

Where S is the search space, typically bounded: S = [xₘᵢₙ, xₘₐₓ]ⁿ

### 2.2 Core PSO Mathematics

#### Particle Representation
Each particle i in the swarm has:
- **Position**: xᵢ(t) ∈ ℝⁿ (current solution)
- **Velocity**: vᵢ(t) ∈ ℝⁿ (rate of position change)
- **Personal best**: pᵢ(t) ∈ ℝⁿ (best position found by particle i)
- **Global best**: g(t) ∈ ℝⁿ (best position found by entire swarm)

#### Update Equations

The canonical PSO update equations are:

**Velocity Update:**
```
vᵢ(t+1) = w·vᵢ(t) + c₁·r₁·(pᵢ(t) - xᵢ(t)) + c₂·r₂·(g(t) - xᵢ(t))
```

**Position Update:**
```
xᵢ(t+1) = xᵢ(t) + vᵢ(t+1)
```

Where:
- w = inertia weight (typically 0.4 to 0.9)
- c₁ = cognitive acceleration coefficient (typically 2.0)
- c₂ = social acceleration coefficient (typically 2.0)
- r₁, r₂ = uniform random numbers in [0,1]
- ⊙ denotes element-wise multiplication

### 2.3 Velocity Components Analysis

The velocity equation consists of three terms:

1. **Inertia term**: w·vᵢ(t)
   - Provides momentum
   - Controls exploration vs exploitation
   - Large w → more exploration
   - Small w → more exploitation

2. **Cognitive term**: c₁·r₁·(pᵢ(t) - xᵢ(t))
   - Pulls particle toward its personal best
   - Represents individual experience
   - Stochastic through r₁

3. **Social term**: c₂·r₂·(g(t) - xᵢ(t))
   - Pulls particle toward global best
   - Represents swarm knowledge
   - Stochastic through r₂

### 2.4 Convergence Analysis

#### Stability Condition
For convergence, the following condition should be satisfied:
```
w > 0.5(c₁ + c₂) - 1
```

#### Constriction Factor Approach (Clerc & Kennedy, 2002)
An alternative formulation using constriction factor χ:

```
vᵢ(t+1) = χ[vᵢ(t) + φ₁·(pᵢ(t) - xᵢ(t)) + φ₂·(g(t) - xᵢ(t))]
```

Where:
- φ₁ = c₁·r₁, φ₂ = c₂·r₂
- φ = φ₁ + φ₂, typically φ > 4
- χ = 2/|2 - φ - √(φ² - 4φ)|

For φ = 4.1, χ ≈ 0.7298

## 3. Standard PSO Algorithm

### 3.1 Basic Algorithm Structure

```
Algorithm: Standard PSO
Input: f (objective function), N (swarm size), T (max iterations)
Output: g (global best position)

1. Initialize:
   For each particle i = 1 to N:
      - Initialize xᵢ randomly in search space
      - Initialize vᵢ randomly (typically small values)
      - Set pᵢ = xᵢ
      - Evaluate f(xᵢ)
   Set g = argmin{f(pᵢ)}, i = 1...N

2. Main Loop:
   For t = 1 to T:
      For each particle i = 1 to N:
         a. Update velocity using PSO equation
         b. Apply velocity clamping if needed
         c. Update position
         d. Apply boundary handling
         e. Evaluate f(xᵢ(t))
         f. Update personal best:
            If f(xᵢ(t)) < f(pᵢ(t-1)):
               pᵢ(t) = xᵢ(t)
         g. Update global best:
            If f(xᵢ(t)) < f(g(t-1)):
               g(t) = xᵢ(t)

3. Return g
```

### 3.2 Important Implementation Details

#### Velocity Clamping
To prevent velocity explosion:
```
vᵢ,d = min(max(vᵢ,d, -vₘₐₓ), vₘₐₓ)
```
Typically: vₘₐₓ = α(xₘₐₓ - xₘᵢₙ), where α ∈ [0.1, 1.0]

#### Boundary Handling Strategies

1. **Absorbing walls**: 
   ```
   if xᵢ,d > xₘₐₓ,d: xᵢ,d = xₘₐₓ,d; vᵢ,d = 0
   ```

2. **Reflecting walls**:
   ```
   if xᵢ,d > xₘₐₓ,d: xᵢ,d = xₘₐₓ,d; vᵢ,d = -vᵢ,d
   ```

3. **Periodic/Toroidal**:
   ```
   xᵢ,d = xₘᵢₙ,d + mod(xᵢ,d - xₘᵢₙ,d, xₘₐₓ,d - xₘᵢₙ,d)
   ```

## 4. PSO Variants

### 4.1 Inertia Weight Strategies

#### Linear Decreasing Inertia Weight (LDIW)
```
w(t) = wₘₐₓ - (wₘₐₓ - wₘᵢₙ) · t/T
```
Typically: wₘₐₓ = 0.9, wₘᵢₙ = 0.4

#### Adaptive Inertia Weight
```
w(t) = wₘᵢₙ + (wₘₐₓ - wₘᵢₙ) · exp(-a·t)
```

### 4.2 Topology Variants

#### Global Best (gbest) PSO
- All particles connected
- Fast convergence
- Risk of premature convergence

#### Local Best (lbest) PSO
- Particles connected in neighborhoods
- Ring topology: particle i connected to i-1 and i+1
- Update equation uses neighborhood best lᵢ instead of g:
```
vᵢ(t+1) = w·vᵢ(t) + c₁·r₁·(pᵢ(t) - xᵢ(t)) + c₂·r₂·(lᵢ(t) - xᵢ(t))
```

#### Von Neumann Topology
- 2D grid structure
- Each particle connected to 4 neighbors
- Better exploration

### 4.3 Advanced Variants

#### Comprehensive Learning PSO (CLPSO)
- Particles learn from different exemplars
- Better for multimodal problems

#### Adaptive PSO (APSO)
- Dynamically adjusts parameters
- Monitors swarm diversity

#### Quantum PSO (QPSO)
- Quantum-inspired mechanics
- No velocity vector
- Position update based on quantum potential well

## 5. Performance Metrics

### 5.1 Convergence Metrics
- **Best fitness over iterations**: Track f(g(t))
- **Average fitness**: Mean fitness of all particles
- **Standard deviation**: Swarm diversity measure

### 5.2 Diversity Metrics

#### Position Diversity
```
D(t) = (1/N) Σᵢ₌₁ᴺ √(Σⱼ₌₁ⁿ (xᵢ,ⱼ(t) - x̄ⱼ(t))²)
```
Where x̄ⱼ is the mean position in dimension j

#### Velocity Diversity
Similar calculation using velocity vectors

### 5.3 Success Criteria
- **Success rate**: Percentage of runs finding global optimum
- **Function evaluations**: Computational cost
- **Convergence speed**: Iterations to reach threshold

## 6. Test Functions Overview

### 6.1 Unimodal Functions (Single Global Optimum)

#### Sphere Function
```
f(x) = Σᵢ₌₁ⁿ xᵢ²
Domain: [-5.12, 5.12]ⁿ
Global minimum: f(0) = 0
```

#### Rosenbrock Function
```
f(x) = Σᵢ₌₁ⁿ⁻¹ [100(xᵢ₊₁ - xᵢ²)² + (xᵢ - 1)²]
Domain: [-2.048, 2.048]ⁿ
Global minimum: f(1,...,1) = 0
```

### 6.2 Multimodal Functions (Multiple Local Optima)

#### Rastrigin Function
```
f(x) = 10n + Σᵢ₌₁ⁿ [xᵢ² - 10cos(2πxᵢ)]
Domain: [-5.12, 5.12]ⁿ
Global minimum: f(0) = 0
```

#### Ackley Function
```
f(x) = -20exp(-0.2√((1/n)Σᵢ₌₁ⁿ xᵢ²)) - exp((1/n)Σᵢ₌₁ⁿ cos(2πxᵢ)) + 20 + e
Domain: [-32.768, 32.768]ⁿ
Global minimum: f(0) = 0
```

#### Griewank Function
```
f(x) = 1 + (1/4000)Σᵢ₌₁ⁿ xᵢ² - Πᵢ₌₁ⁿ cos(xᵢ/√i)
Domain: [-600, 600]ⁿ
Global minimum: f(0) = 0
```

### 6.3 Fixed-Dimension Functions

#### Shekel's Foxholes (2D)
#### Six-Hump Camel Back (2D)
#### Branin (2D)
#### Goldstein-Price (2D)

## 7. Parameter Tuning Guidelines

### 7.1 Standard Parameter Settings
- **Swarm size**: 20-50 particles (often 30)
- **Inertia weight**: w = 0.7298 (constriction) or 0.9→0.4 (LDIW)
- **Acceleration coefficients**: c₁ = c₂ = 2.0
- **Velocity limit**: vₘₐₓ = 0.2 × range

### 7.2 Problem-Specific Tuning

#### For Unimodal Problems:
- Higher social coefficient (c₂ > c₁)
- Lower inertia weight
- Global topology

#### For Multimodal Problems:
- Balanced coefficients (c₁ ≈ c₂)
- Higher/adaptive inertia
- Local topology
- Larger swarm size

### 7.3 Adaptive Parameter Strategies
- Monitor convergence rate
- Track diversity metrics
- Adjust parameters based on stagnation detection

## 8. Implementation Considerations for C++

### 8.1 Data Structures
```cpp
struct Particle {
    vector<double> position;
    vector<double> velocity;
    vector<double> personal_best;
    double personal_best_fitness;
};

class PSO {
    vector<Particle> swarm;
    vector<double> global_best;
    double global_best_fitness;
    // Parameters: w, c1, c2, bounds, etc.
};
```

### 8.2 Random Number Generation
- Use `<random>` library
- Thread-safe generators for parallel implementation
- Seed management for reproducibility

### 8.3 Optimization Techniques
- Vectorization with SIMD instructions
- Parallel evaluation of particles (OpenMP/threads)
- Early termination criteria
- Memory pooling for large swarms

### 8.4 Modularity Suggestions
- Abstract base class for optimization algorithms
- Function interface for objective functions
- Strategy pattern for:
  - Topology implementations
  - Boundary handling
  - Parameter adaptation
  - Initialization methods

## 9. Extensions to Other Swarm Algorithms

### 9.1 Common Framework
Most swarm algorithms share:
- Population-based search
- Position update rules
- Information sharing mechanisms
- Balance of exploration/exploitation

### 9.2 Related Algorithms to Explore

#### Artificial Bee Colony (ABC)
- Three bee types: employed, onlooker, scout
- Different search mechanisms
- Good for constrained optimization

#### Cuckoo Search (CS)
- Lévy flights for exploration
- Parasitic breeding behavior
- Fewer parameters than PSO

#### Firefly Algorithm (FA)
- Attraction based on brightness
- All-to-all comparison
- Good for multimodal problems

#### Ant Colony Optimization (ACO)
- Discrete/combinatorial problems
- Pheromone-based communication
- Path construction approach

## 10. References and Further Reading

### Essential Papers
1. Kennedy, J., & Eberhart, R. (1995). "Particle swarm optimization"
2. Clerc, M., & Kennedy, J. (2002). "The particle swarm - explosion, stability, and convergence"
3. Shi, Y., & Eberhart, R. (1998). "A modified particle swarm optimizer"
4. Liang, J. J., et al. (2006). "Comprehensive learning particle swarm optimizer"

### Books
- "Particle Swarm Optimization" by Clerc (2006)
- "Swarm Intelligence" by Kennedy & Eberhart (2001)
- "Nature-Inspired Metaheuristic Algorithms" by Yang (2010)

### Online Resources
- PSO Central: Repository of PSO research
- CEC Benchmark Functions: Standard test suites
- DEAP library: Reference implementations (Python, but good for algorithms)

## Next Steps

1. **Implement Basic PSO**: Start with standard gbest PSO for sphere function
2. **Add Test Functions**: Implement the benchmark suite systematically
3. **Experiment with Variants**: Try different topologies and parameter strategies
4. **Performance Analysis**: Track metrics, create visualization tools
5. **Extend to Other Algorithms**: Use PSO framework as template

Remember: Understanding the balance between exploration (searching new areas) and exploitation (refining known good areas) is key to mastering swarm optimization!