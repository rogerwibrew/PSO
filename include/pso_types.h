#ifndef PSO_TYPES_H
#define PSO_TYPES_H

#include <string>
#include <vector>

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

struct OptimisationResult {
  std::vector<double> best_position;
  double best_fitness;
  int iterations;
  int function_evaluations;
  std::vector<double> fitness_history;
  bool coverged;
  std::string convergence_reason;
};

#endif // PSO_TYPES_H
