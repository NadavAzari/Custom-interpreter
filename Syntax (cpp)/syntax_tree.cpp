#include "syntax_tree.h"

syntax_tree::syntax_tree(expression* exp, errors_bag* _bag) : root(exp), bag(_bag) {};
syntax_tree::~syntax_tree() { delete root; delete bag; }
expression* syntax_tree::get_root() { return root; }
errors_bag* syntax_tree::get_errors() { return bag; }