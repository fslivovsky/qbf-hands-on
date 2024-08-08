#include <cassert>
#include "depqbf.h"

int main() {
  DepQBF solver;
  
  solver.configure("--dep-man=simple");
  solver.configure("--incremental-use");
  
  solver.new_scope(QDPLL_QTYPE_FORALL, 1);
  solver.add_variable(1);
  solver.add_variable(99);
  solver.add_variable(0); // Close scope
  
  assert(solver.is_var_declared(1));
  assert(solver.is_var_declared(99));
  assert(!solver.is_var_declared(50));
  
  QDPLLResult result = solver.solve();
  assert(result == QDPLL_RESULT_SAT || result == QDPLL_RESULT_UNSAT);
  
  return 0;
}