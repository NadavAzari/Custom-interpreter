#pragma once
#include "expressions.h"
#include "syntax.h"
#include "token.h"
#include <iostream>

class evaluator {
private:
	errors_bag* bag = new errors_bag;
	std::string evaluate_expression(expression*, bool=true);
	bool exit = false;
public:
	bool has_errors();
	error* get_error(int);
	std::string evaluate(syntax_tree*,bool=true);
	~evaluator();
};