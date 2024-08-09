#ifndef DEPQBF_H
#define DEPQBF_H

#include <string>
#include <vector>
#include <stdexcept>

extern "C" {
  #include "qdpll.h"
}

class DepQBF {
public:
// Exception class for DepQBF-related errors
  class DepQBF_exception : public std::runtime_error {
  public:
    explicit DepQBF_exception(const std::string& message)
      : std::runtime_error("DepQBF Error: " + message) {}
  };

  DepQBF();
  ~DepQBF();
  
  void configure(const std::string& option);
  void add_quantifier_block(bool is_existential, const std::vector<int>& variables);
  void add_clause(const std::vector<int>& clause);
  bool solve(const std::vector<int>& assumptions);
  std::vector<int> get_outermost_model();
  std::vector<int> get_core();
  void reset();

private:
  int get_value(int var);

  QDPLL* solver;
  std::vector<int> outermost_block;
};

#endif // DEPQBF_H