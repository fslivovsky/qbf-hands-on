#ifndef DEPQBF_H
#define DEPQBF_H

#include <string>

extern "C" {
  #include "qdpll.h"
}

class DepQBF {
public:
  DepQBF();
  ~DepQBF();
  
  void configure(const std::string& option);
  void new_scope(QDPLLQuantifierType qtype, int level);
  void add_variable(int var);
  bool is_var_declared(int var) const;
  QDPLLAssignment get_value(int var);
  QDPLLResult solve();

private:
  QDPLL* solver;
};

#endif // DEPQBF_H