#include "function.h"
#include "global.h"
#include "evaluator.h"
#include <cstdlib>
#include <ctime>


bool function::done = false;

function::function(std::string _name, std::vector<std::string> params, std::vector<syntax_tree*> _lines) : name(_name), params_names(params), lines(_lines) { mem = Memory::create(); }

void function::handle_line(syntax_tree* line, evaluator ev, std::string& returnValue) {
	if (line->get_root()->get_kind() == IF_EXPRESSION) {
		if_expression* if_exp = dynamic_cast<if_expression*>(line->get_root());
		syntax_tree* cond = new syntax_tree(if_exp->get_condition(), new errors_bag);
		if (ev.evaluate(cond) == syntax::TRUE_KEYWORD) {
			for (int i = 0; i < if_exp->get_lines().size(); ++i) {
				handle_line(if_exp->get_lines()[i], ev, returnValue);
				if (returnValue != "") {
					//delete cond;  
					break;
				}
			}
			delete cond;
		}
		else {
			delete cond;
			return;
		}
	}
	if (line->get_root()->get_kind() != RETURN_EXPRESSION) {
		//if (returnValue != "") return;
		ev.evaluate(line);
	}
	else {
		expression* e = ((return_expression*)line->get_root())->get_exp();
		syntax_tree* tr = new syntax_tree(e, new errors_bag);

		returnValue = ev.evaluate(tr);
	}
}

std::string function::execute(std::vector<syntax_tree*> params) {
	Memory* lastMem = CURRENT_MEM;
	std::string returnValue = "";
	bool lastInFunction = IN_FUNCTION;
	evaluator ev;
	for (int i = 0; i < params.size(); ++i) {
		CURRENT_MEM = lastMem;
		std::string val = ev.evaluate(params[i],false);
		CURRENT_MEM = mem;
		variable::create(params_names[i], val);
	}
	IN_FUNCTION = true;
	WITHNIN_STATEMENT = true;
	CURRENT_MEM = mem;


	for (int i = 0; i < lines.size(); ++i) {
		
		auto line = lines[i];

		if (line->get_root()->get_kind() != RETURN_EXPRESSION) {
			if (line->get_root()->get_kind() != IF_EXPRESSION) {
				ev.evaluate(line,false);
			}
			else {
				returnValue = dynamic_cast<if_expression*>(line->get_root())->execute();
			}

		}
		else {
			expression* e = ((return_expression*)line->get_root())->get_exp();
			syntax_tree* tr = new syntax_tree(e, new errors_bag);
			returnValue = ev.evaluate(tr,false);
		}

		if (returnValue != "") break;
	}

	CURRENT_MEM = lastMem;
	WITHNIN_STATEMENT = STATES.size() > 0;
	IN_FUNCTION = lastInFunction;

	mem->clear_vars();
	return returnValue;
}

function::~function() {
	for (int i = 0; i < lines.size(); ++i) {
		delete lines[i];
	}
}

int function::get_params_size() { return params_names.size(); }

void function::print(syntax_tree* value) {
	evaluator ev;
	auto v = ev.evaluate(value,false);
	if (variable::typeof(v) == STRING_VAR) {
		v = v.substr(1, v.size() - 2);
	}
	std::cout << v << "\n";
}

std::string function::input(std::string p) {
	p = p.substr(1, p.size() - 2);
	std::cout << p;
	std::string rValue;
	std::getline(std::cin, rValue);
	return rValue;
}

int function::rand(int s, int e) {
	if (!done) {
		srand(time(NULL));
		done = true;
	}
	return (std::rand() % (e-s+1)) + s;
}

std::string function::built_in(std::string name,std::vector<syntax_tree*> params, errors_bag* bag) {
	if (name == syntax::PRINT_FUNC) {
		if (params.size() != 1) {
			bag->add_error(new error(syntax::PRINT_FUNC + " function expects 1 parameters but " + std::to_string(params.size()) + " were given"));
			return "";
		}
		function::print(params[0]);
		return "";
	}
	else if (name == syntax::RAND_FUNC) {
		if (params.size() != 2) {
			bag->add_error(new error(syntax::RAND_FUNC + " function expects 2 parameters but " + std::to_string(params.size()) + " were given"));
			return "";
		}
		
		evaluator ev;
		auto t1 = variable::typeof(ev.evaluate(params[0],false));
		auto t2 = variable::typeof(ev.evaluate(params[1], false));
		if (t1 != INTEGER || t2 != INTEGER) {
			bag->add_error(new error(syntax::RAND_FUNC + " function expects 2 integers"));
			return "";
		}

		int s, e;
		s = std::stoi(ev.evaluate(params[0], false));
		e = std::stoi(ev.evaluate(params[1], false));

		return std::to_string(function::rand(s,e));
		return "";
	}
	else if (name == syntax::INPUT_FUNC) {
		if (params.size() != 1) {
			bag->add_error(new error(syntax::INPUT_FUNC + " function expects 1 parameters but " + std::to_string(params.size()) + " were given"));
			return "";
		}
		evaluator ev;
		auto t = variable::typeof(ev.evaluate(params[0], false));

		if (t != STRING_VAR) {
			bag->add_error(new error(syntax::INPUT_FUNC + " function expects a string"));
			return "";
		}

		return function::input(ev.evaluate(params[0], false));

	}
	else if (name == syntax::TO_NUM_FUNC) {
		if (params.size() != 1) {
			bag->add_error(new error(syntax::TO_NUM_FUNC + " function expects 1 parameters but " + std::to_string(params.size()) + " were given"));
			return "";
		}
		evaluator ev;
		std::string sVal = ev.evaluate(params[0], false);
		if (variable::typeof(sVal) != STRING_VAR) {
			bag->add_error(new error(syntax::TO_NUM_FUNC + " function expects a string"));
			return "";
		}
		sVal = sVal.substr(1, sVal.size() - 2);
		auto t = variable::typeof(sVal);
		if (t != INTEGER && t != FLOAT_VAR) {
			bag->add_error(new error("'"+sVal+"' is not a valid INTEGER or FLOAT value"));
			return "";
		}
		return sVal;
	}
	else if (name == syntax::TO_STRING_FUNC) {
		if (params.size() != 1) {
			bag->add_error(new error(syntax::TO_STRING_FUNC + " function expects 1 parameters but " + std::to_string(params.size()) + " were given"));
			return "";
		}
		evaluator ev;
		std::string sVal = ev.evaluate(params[0], false);
		if (variable::typeof(sVal) == STRING_VAR) {
			return sVal;
		}
		return syntax::QUOTES + sVal + syntax::QUOTES;
	}
	else if (name == syntax::IS_EXIST) {
		if (params.size() != 1) {
			bag->add_error(new error(syntax::IS_EXIST + " function expects 1 parameters but " + std::to_string(params.size()) + " were given"));
			return "";
		}
		evaluator ev;
		std::string sVal = ev.evaluate(params[0], false);
		if (variable::typeof(sVal) != STRING_VAR) {
			bag->add_error(new error(syntax::IS_EXIST + " function expects a string"));
			return "";
		}

		sVal = sVal.substr(1, sVal.size() - 2);

		if (CURRENT_MEM->has_var(sVal)) return syntax::TRUE_KEYWORD;
		return syntax::FALSE_KEYWORD;
	}
	else if (name == syntax::EVAL) {
		if (params.size() != 1) {
			bag->add_error(new error(syntax::EVAL + " function expects 1 parameters but " + std::to_string(params.size()) + " were given"));
			return "";
		}
		evaluator ev;
		std::string sVal = ev.evaluate(params[0], false);
		if (variable::typeof(sVal) != STRING_VAR) {
			bag->add_error(new error(syntax::EVAL + " function expects a string"));
			return "";
		}
		sVal = sVal.substr(1, sVal.size() - 2);
		parser p = parser(sVal);
		syntax_tree* t = p.parse();
		if (t->get_root()->get_kind() != LITTERAL_EXPRESSION &&t->get_root()->get_kind() != BINARY_EXPRESSION && t->get_root()->get_kind() != BOOLEAN_EXPRESSION) {
			bag->add_error(new error("Invalid expression."));
			return "";
		}
		auto result = ev.evaluate(t);
		delete t;
		return result;
		
	}
	return "";
}