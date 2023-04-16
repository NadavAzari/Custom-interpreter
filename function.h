#pragma once
#include <iostream>
#include "expressions.h"
#include "memory.h"
#include "evaluator.h"

class Memory;
class evaluator;

class function {
private:
	std::string name;
	std::vector<std::string> params_names;
	std::vector<syntax_tree*> lines;
	Memory* mem;
	static void print(syntax_tree* param);
	static int rand(int start, int end);
	static std::string input(std::string);
	static std::string num(std::string);


	static bool done;
public:
	static std::string built_in(std::string name, std::vector<syntax_tree*>, errors_bag*);
	static bool is_built_in(std::string name) { return name == syntax::PRINT_FUNC || name == syntax::RAND_FUNC || name == syntax::INPUT_FUNC || name == syntax::TO_NUM_FUNC || name == syntax::TO_STRING_FUNC || name == syntax::IS_EXIST || name == syntax::EVAL; };
	function(std::string, std::vector<std::string>, std::vector<syntax_tree*>);
	std::string execute(std::vector<syntax_tree*>);
	int get_params_size();
	void handle_line(syntax_tree*, evaluator, std::string&);
	~function();
};