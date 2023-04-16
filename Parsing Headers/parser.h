#pragma once
#include <iostream>
#include "token.h"
#include "lexer.h"
#include "syntax.h"
#include <vector>
#include "expressions.h"
#include "syntax_tree.h"

class syntax_tree;
class expression;

class parser {
private:
	expression* parse_expression(int=0);
	expression* parse_primary();
	std::vector<token> tokens;
	errors_bag* bag;
	token match(syntax_kind);
	token peek(int);
	int pos;
public:
	syntax_tree* parse();
	parser(std::string);
};