#include <cassert>
#include <iostream>

#include "depqbf.h"

int main() {
  try {
    DepQBF solver;
    
    solver.configure("--dep-man=simple");
    solver.configure("--incremental-use");
    
    solver.add_quantifier_block(true, {1, 2});
    solver.add_quantifier_block(true, {4, 5});
    solver.add_clause({1, 4});
    solver.add_clause({-1, 5});
    
    auto result = solver.solve({});
    std::cout << "Result: " << result << std::endl;
    if (result) {
      auto model = solver.get_outermost_model();
      std::cout << "Model: ";
      for (auto l: model) {
        std::cout << l << " ";
      }
      std::cout << std::endl;
    }
    return 0;
  } catch (DepQBF::DepQBF_exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}