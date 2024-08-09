#include "depqbf.h"

#include <cstring>
#include <cassert>
#include <iostream>

DepQBF::DepQBF() {
  solver = qdpll_create();
  qdpll_configure(solver, "--no-dynamic-nenofex");
}

DepQBF::~DepQBF() {
  qdpll_delete(solver);
}

void DepQBF::configure(const std::string& option) {
  char* cstr = new char [option.length()+1];
  std::strcpy (cstr, option.c_str());
  char* error = qdpll_configure(solver, cstr);
  delete[] cstr;
  if (error) {
    throw DepQBF_exception(error);
  }
}

void DepQBF::add_quantifier_block(bool is_existential, const std::vector<int>& variables) {
  if (variables.empty()) {
    throw DepQBF_exception("Empty quantifier block");
  }
  QDPLLQuantifierType qtype = is_existential ? QDPLL_QTYPE_EXISTS : QDPLL_QTYPE_FORALL;
  auto max_scope_nesting = qdpll_get_max_scope_nesting(solver);
  qdpll_new_scope_at_nesting (solver, qtype, max_scope_nesting + 1);
  for (auto v: variables) {
    qdpll_add(solver, v);
  }
  qdpll_add(solver, 0);
  if (outermost_block.empty()) {
    outermost_block = variables;
  }
}

void DepQBF::add_clause(const std::vector<int>& clause) {
  for (auto l: clause) {
    if (l == 0) {
      throw DepQBF_exception("Zero literals are not allowed in clauses");
    }
    qdpll_add(solver, l);
  }
  qdpll_add(solver, 0);
}

bool DepQBF::solve(const std::vector<int>& assumptions) {
  for (auto l: assumptions) {
    if (l == 0) {
      throw DepQBF_exception("Zero literals are not allowed in assumptions");
    }
    qdpll_assume(solver, l);
  }
  auto result = qdpll_sat(solver);
  if (result != QDPLL_RESULT_SAT && result != QDPLL_RESULT_UNSAT) {
    throw DepQBF_exception("Unexpected result from qdpll_sat: " + std::to_string(result));
  }
  return result == QDPLL_RESULT_SAT;
}

std::vector<int> DepQBF::get_outermost_model() {
  std::vector<int> model;
  for (auto v: outermost_block) {
    int val = get_value(v);
    if (val != 0) {
      model.push_back(val);
    }
  }
  return model;
}

std::vector<int> DepQBF::get_core() {
  LitID* core_lits = qdpll_get_relevant_assumptions (solver);
  std::vector<int> core;
  LitID *p;
  for (p = core_lits; *p; p++) {
    core.push_back(*p);
  }
  free(core_lits);
  return core;
}

void DepQBF::reset() {
  qdpll_reset(solver);
}

int DepQBF::get_value(int var) {
  QDPLLAssignment val = qdpll_get_value(solver, var);
  switch (val) {
    case QDPLL_ASSIGNMENT_TRUE:
      return var;
    case QDPLL_ASSIGNMENT_FALSE:
      return -var;
    default:
      return 0;
  }
}

