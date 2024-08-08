#include "depqbf.h"

#include <cstring>

DepQBF::DepQBF() {
  solver = qdpll_create();
}

DepQBF::~DepQBF() {
  qdpll_delete(solver);
}

void DepQBF::configure(const std::string& option) {
  char * cstr = new char [option.length()+1];
  std::strcpy (cstr, option.c_str());
  qdpll_configure(solver, cstr);
  delete[] cstr;
}

void DepQBF::new_scope(QDPLLQuantifierType qtype, int level) {
  qdpll_new_scope_at_nesting(solver, qtype, level);
}

void DepQBF::add_variable(int var) {
  qdpll_add(solver, var);
}

QDPLLAssignment DepQBF::get_value(int var) {
  return qdpll_get_value(solver, var);
}

bool DepQBF::is_var_declared(int var) const {
  return qdpll_is_var_declared(solver, var);
}

QDPLLResult DepQBF::solve() {
  return qdpll_sat(solver);
}
