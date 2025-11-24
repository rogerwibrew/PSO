# PSO Implementation Reference

## 1. Core Algorithm

### 1.1 Essential Components
- **Swarm**: Collection of particles
- **Particle**: Position vector, velocity vector, personal best position
- **Global Best**: Best position found by entire swarm
- **Objective Function**: f(x) to minimize/maximize

### 1.2 Main Loop Structure
```
1. Initialize swarm
2. Evaluate all particles
3. While not converged:
   a. Update velocities
   b. Update positions
   c. Handle boundaries
   d. Evaluate fitness
   e. Update personal bests
   f. Update global best
4. Return best solution
```

## 2. Mathematical Equations

### 2.1 Standard PSO Update
**Velocity Update:**
```
v[i,d](t+1) = w * v[i,d](t) + 
              c1 * r1 * (pbest[i,d] - x[i,d](t)) +
              c2 * r2 * (gbest[d] - x[i,d](t))
```

**Position Update:**
```
x[i,d](t+1) = x[i,d](t) + v[i,d](t+1)
```

### 2.2 Parameters
- `w`: inertia weight [0.4, 0.9]
- `c1`: cognitive coefficient (typically 2.0)
- `c2`: social coefficient (typically 2.0)
- `r1, r2`: random [0,1]

### 2.3 Constriction Factor (Alternative)
```
χ = 2 / |2 - φ - sqrt(φ² - 4φ)|
where φ = c1 + c2, typically φ > 4
```

## 3. Implementation Checklist

### 3.1 Initialization
- [ ] Random positions within bounds
- [ ] Small random velocities (±10% of range)
- [ ] Set personal best = initial position
- [ ] Evaluate initial fitness
- [ ] Find initial global best

### 3.2 Velocity Clamping
```
v_max = α * (x_max - x_min)
α typically 0.1 to 0.2
Clamp: v = min(max(v, -v_max), v_max)
```

### 3.3 Boundary Handling Options

**Absorbing:**
```
if x > x_max: x = x_max, v = 0
if x < x_min: x = x_min, v = 0
```

**Reflecting:**
```
if x > x_max: x = x_max, v = -v
if x < x_min: x = x_min, v = -v
```

**Periodic:**
```
x = x_min + modulo(x - x_min, x_max - x_min)
```

## 4. Standard Parameters

### 4.1 Typical Values
- Swarm size: 20-50 (often 30)
- Max iterations: 1000-5000
- Dimensions: problem-specific

### 4.2 Inertia Weight Strategies

**Constant:**
```
w = 0.7298 (with constriction)
```

**Linear Decreasing:**
```
w(t) = w_max - (w_max - w_min) * (t / max_iter)
w_max = 0.9, w_min = 0.4
```

## 5. Test Functions

### 5.1 Sphere (Unimodal)
```
f(x) = Σ(x[i]²)
Domain: [-5.12, 5.12]^n
Global min: f(0,0,...) = 0
```

### 5.2 Rosenbrock (Unimodal, Narrow Valley)
```
f(x) = Σ[100(x[i+1] - x[i]²)² + (x[i] - 1)²]
Domain: [-2.048, 2.048]^n
Global min: f(1,1,...) = 0
```

### 5.3 Rastrigin (Multimodal)
```
f(x) = 10n + Σ[x[i]² - 10*cos(2π*x[i])]
Domain: [-5.12, 5.12]^n
Global min: f(0,0,...) = 0
```

### 5.4 Ackley (Multimodal)
```
f(x) = -20*exp(-0.2*sqrt((1/n)Σx[i]²)) - 
       exp((1/n)Σcos(2π*x[i])) + 20 + e
Domain: [-32.768, 32.768]^n
Global min: f(0,0,...) = 0
```

### 5.5 Griewank (Multimodal)
```
f(x) = 1 + (1/4000)Σx[i]² - Π cos(x[i]/sqrt(i))
Domain: [-600, 600]^n
Global min: f(0,0,...) = 0
```

### 5.6 Schwefel (Multimodal, Deceptive)
```
f(x) = 418.9829*n - Σ[x[i]*sin(sqrt(|x[i]|))]
Domain: [-500, 500]^n
Global min: f(420.9687,...) = 0
```

## 6. Convergence Criteria

### 6.1 Options
- Maximum iterations reached
- Fitness below threshold
- No improvement for N iterations
- Swarm diversity below threshold

### 6.2 Diversity Measure
```
diversity = (1/N) * Σ ||x[i] - x_mean||
```

## 7. Performance Metrics

### 7.1 Track During Optimization
- Best fitness per iteration
- Average fitness
- Diversity
- Function evaluations

### 7.2 Multi-Run Statistics
- Success rate (% reaching optimum)
- Mean best fitness
- Standard deviation
- Average convergence iteration

## 8. Topology Variants

### 8.1 Global (gbest)
- All particles connected
- Use global best in velocity update
- Fast convergence, risk of premature convergence

### 8.2 Ring (lbest)
- Each particle connected to k neighbors
- Use local neighborhood best
- Slower convergence, better exploration

### 8.3 Von Neumann
- 2D grid structure
- 4 neighbors per particle
- Good balance

## 9. Algorithm Pseudocode

```
function PSO(objective, bounds, params):
    swarm = initialize_particles(params.swarm_size, bounds)
    gbest = find_best(swarm)
    
    for iter in 1 to params.max_iterations:
        for each particle p in swarm:
            update_velocity(p, gbest, params)
            clamp_velocity(p, params.v_max)
            update_position(p)
            handle_boundaries(p, bounds)
            
            fitness = evaluate(p.position)
            if fitness < p.best_fitness:
                p.best_position = p.position
                p.best_fitness = fitness
            
            if fitness < gbest_fitness:
                gbest = p.position
                gbest_fitness = fitness
        
        if converged():
            break
    
    return gbest
```

## 10. Implementation Notes

### 10.1 Random Number Generation
- Need uniform distribution [0,1]
- Different random values for each dimension
- Seed management for reproducibility

### 10.2 Memory Considerations
- Position arrays: N particles × D dimensions
- Velocity arrays: N particles × D dimensions
- Personal bests: N particles × D dimensions
- Global best: 1 × D dimensions

### 10.3 Computational Complexity
- Per iteration: O(N × D) for updates
- Function evaluations: N per iteration
- Memory: O(N × D)

## 11. Common Pitfalls

### 11.1 Issues to Avoid
- Velocity explosion (use clamping)
- All particles converging to local optimum (use topology)
- Boundary violations (proper handling)
- Parameter imbalance (c1 ≈ c2)

### 11.2 Debugging Tips
- Visualize 2D problems
- Track diversity over time
- Monitor velocity magnitudes
- Check boundary handling
- Verify random number generation

## 12. Extensions to Consider

### 12.1 After Basic Implementation
- Adaptive parameters
- Different topologies
- Parallel evaluation
- Constraint handling
- Multi-objective optimization

### 12.2 Other Swarm Algorithms
- **ABC**: Employed/Onlooker/Scout bees
- **Cuckoo**: Lévy flights, egg laying
- **Firefly**: Brightness-based attraction
- **Ant Colony**: Pheromone trails

## 13. Useful Constants

```cpp
const double PI = 3.14159265358979323846;
const double E = 2.71828182845904523536;
```

## 14. Quick Reference

### Standard Setup
```
Swarm size: 30
w: 0.7298 or 0.9→0.4 (linear)
c1, c2: 2.0
v_max: 0.2 × range
Iterations: 1000
```

### For Multimodal Problems
- Use ring topology (3-5 neighbors)
- Higher swarm size (40-50)
- Linear decreasing inertia
- More iterations (2000-5000)