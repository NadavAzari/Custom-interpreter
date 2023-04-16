#include <iostream>
#include <string>
#include <fstream>

#include "syntax.h"
#include "parser.h"
#include "evaluator.h"
#include "syntax_tree.h"
#include "memory.h"
#include "global.h"
#include "expressions.h"

#define __NO_FILE__ 1

statement_expression* get_state() { return STATES[STATES.size() - 1]; }


void handle_statement(std::string line) {
	std::string no_space = line;
	no_space.erase(std::remove_if(no_space.begin(), no_space.end(), std::isspace), no_space.end());
	statement_expression* CURRENT_STATE = get_state();

	if (no_space == syntax::LEAVE_STATEMENT) {
		STATES.pop_back();
		WITHNIN_STATEMENT = STATES.size() > 0 || IN_FUNCTION;
		if ((dynamic_cast<if_expression*>(CURRENT_STATE) || dynamic_cast<while_loop_expression*>(CURRENT_STATE))  && !WITHNIN_STATEMENT) {
			CURRENT_STATE->execute();
			delete CURRENT_STATE;
		}
		else if (function_expression* func_exp = dynamic_cast<function_expression*>(CURRENT_STATE)) {
			if (!WITHNIN_STATEMENT) {
				function* func = new function(func_exp->get_identifier().get_text(), func_exp->get_params(), func_exp->get_lines());
				CURRENT_MEM->add_function(func_exp->get_identifier().get_text(), func);
			}
		}
		return;
	}

	parser _parser(line);
	syntax_tree* tree = _parser.parse();
	if (tree->get_errors()->size() != 0) {
		std::cout << tree->get_errors()->get_error(0)->get_message() << "\n";
		delete CURRENT_STATE;
		STATES.pop_back();
		WITHNIN_STATEMENT = STATES.size() > 0;
	}
	else {
		if(tree->get_root()->get_kind() == IF_EXPRESSION) {
			STATES.push_back((statement_expression*)tree->get_root());
			CURRENT_STATE->add_line(tree);
		}
		else {
			CURRENT_STATE->add_line(tree);
		}
	}
}


void execute_line(std::string line) {
	parser _parser(line);
	syntax_tree* tree = _parser.parse();
	evaluator* _evaluator = new evaluator;
	if (tree->get_errors()->size() != 0) {
		//std::cout << "\u001b[31m";
		std::cout << tree->get_errors()->get_error(0)->get_message() << "\n";
		//std::cout << "\u001b[37m";
	}
	else {
		std::string res = _evaluator->evaluate(tree);
		if (_evaluator->has_errors()) {
			//std::cout << "\u001b[31m";
			std::cout << _evaluator->get_error(0)->get_message() << "\n";
			//std::cout << "\u001b[37m";
		}
		if (res != "")
			std::cout << res << "\n";
	}
	delete _evaluator;
	delete tree;
}

void execute_file(char* name) {
	std::ifstream file;
	//file.open("D:/Visual studio projects/Interpreter/x64/Debug/code.lbj");
	file.open(name);
	std::string line;
	if (file.is_open()) {
		while (file) {
			if (file.eof()) break;
			std::getline(file, line);
			if (line == "") {
				CURRENT_LINE++;
				continue;
			}
			if (WITHNIN_STATEMENT) {
				handle_statement(line);
			}
			else {
				execute_line(line);
			}
			CURRENT_LINE++;
		}
	}
	file.close();
}


void execute_file() {
	std::ifstream file;
	file.open("D:\\Visual studio projects\\Interpreter\\x64\\Debug\\eq_gen.lbj");
	//file.open(name);
	std::string line;
	if (file.is_open()) {
		while (file) {
			if (file.eof()) break;
			std::getline(file, line);
			if (line == "") {
				CURRENT_LINE++;
				continue;
			}
			if (WITHNIN_STATEMENT) {
				handle_statement(line);
			}
			else {
				execute_line(line);
			}
			CURRENT_LINE++;
		}
	}
	file.close();
}

int main(int argc, char* argv[]) {
	syntax::initialize();
	Memory::initialize();
	//std::cout << "Lbj language version(0.0.1)\n";

	WITHNIN_STATEMENT = false;
	CURRENT_MEM = Memory::main();

	//execute_file();
	//return 0;

	if (argc != __NO_FILE__) {
		WITHNIN_FILE = true;
		char* file_name = argv[1];

		execute_file(file_name);
		return 0;
	}

	WITHNIN_FILE = false;
	while (1) {
		if (!WITHNIN_STATEMENT) {
			std::string line;
			std::cout << ">> ";
			std::getline(std::cin, line);
			execute_line(line);
		}
		else {
			std::cout << "...";
			std::string line;


			std::getline(std::cin, line);
			handle_statement(line);
			
		}
	}
	return 0;
}