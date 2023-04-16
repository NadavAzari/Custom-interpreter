#pragma once
#include "token.h"
#include "parser.h"
#include "errors.h"


class lexer {
private:
	std::string line;
	int pos;
	errors_bag* bag;
	token next();
	void add_error();


public:
	lexer(std::string);



friend class parser;



};