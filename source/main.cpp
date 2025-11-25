#include <iostream>
#include <string>
#include <tuple>
#include "pso_types.h"

int main(int argc, char* argv[]) {
    std::cout << "Particle Swarm Optimization (PSO) Application" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Build successful! C++17 features enabled." << std::endl;
    std::cout << std::endl;

    // Demonstrate C++17 feature (structured bindings)
    auto [x, y, z] = std::make_tuple(1.0, 2.0, 3.0);
    std::cout << "Testing C++17 structured bindings: "
              << "x=" << x << ", y=" << y << ", z=" << z << std::endl;

    std::cout << std::endl;
    std::cout << "Ready for PSO implementation!" << std::endl;

    return 0;
}
