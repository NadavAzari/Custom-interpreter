#pragma once
#include "expressions.h"
#include "parser.h"
#include "errors.h"
#include <iostream>

class expression;

class syntax_tree {
private:
	expression* root;
	errors_bag* bag;
	
public:
	syntax_tree(expression*, errors_bag*);
	expression* get_root();
	errors_bag* get_errors();
	~syntax_tree();
};