#include "evaluator.h"
#include "variable.h"
#include "memory.h"
#include "global.h"
#include "syntax_tree.h"
#include <math.h>
#include <map>

class if_expression;

bool to_boolean(std::string s) {
	return s == syntax::TRUE_KEYWORD;
}

bool is_a_number(std::string val) {
	auto type = variable::typeof(val);
	return type == INTEGER || type == FLOAT_VAR;
}

bool is_bitwise(syntax_kind op) {
	if (op == OR_TOKEN || op == AND_TOKEN || op == XOR_TOKEN) {
		return true;
	}
	return false;
}

std::map<types, std::string> types_to_string = {
	{INTEGER,"INT32"},
	{FLOAT_VAR,"FLOAT"},
	{BOOLEAN_VAR,"BOOLEAN"},
	{STRING_VAR,"STRING"}
};

std::string evaluate_binary(syntax_kind op, float l, float r) {
	switch (op) {
	case PLUS_TOKEN:
		return std::to_string(l + r);

	case MINUS_TOKEN:
		return std::to_string(l - r);
	case STAR_TOKEN:
		return std::to_string(l * r);
	case SLASH_TOKEN:
		return std::to_string(l / r);
	case POWER_TOKEN:
		return std::to_string(pow(l, r));
	case XOR_TOKEN:
		return std::to_string((int)l ^ (int)r);
	case AND_TOKEN:
		return std::to_string((int)l & (int)r);
	case OR_TOKEN:
		return std::to_string((int)l | (int)r);
	default:
		return "";
	}

}

std::string evaluator::evaluate_expression(expression* exp, bool first) {

	if (function_call_expression* func_call = dynamic_cast<function_call_expression*>(exp)) {
		token identifier = func_call->get_identifier();
		function* func = nullptr;
		if (CURRENT_MEM->has_function(identifier.get_text())) {
			func = CURRENT_MEM->get_function(identifier.get_text());
		}
		else if (Memory::main()->has_function(identifier.get_text())) {
			func = Memory::main()->get_function(identifier.get_text());
		}
		else if (function::is_built_in(identifier.get_text())) {
			 std::string val = function::built_in(identifier.get_text(), func_call->get_params(),bag);
			 if (bag->size() != 0) {
				 exit = true;
				 return "";
			 }
			 if (identifier.get_text() == syntax::INPUT_FUNC && variable::typeof(val) != STRING_VAR) {
				 return syntax::QUOTES + val + syntax::QUOTES;
			 }
			 return val;
		}
		else {
			exit = true;
			bag->add_error(new error("Undefined function at line " + std::to_string(CURRENT_LINE) + " '" + identifier.get_text() + "'"));
			return "";
		}

		if (func) {
			if (func->get_params_size() != func_call->get_params().size()) {
				exit = true;
				bag->add_error(new error("the function '" + identifier.get_text() + "' is asking for " + std::to_string(func->get_params_size()) + " parameters but " + std::to_string(func_call->get_params().size()) + " were passed to it"));
				return "";
			}
			std::vector<syntax_tree*> params = func_call->get_params();
			std::string r = func->execute(params);
			return r;
		}
		
	}	
	if (function_expression* func_state = dynamic_cast<function_expression*>(exp)) {
		WITHNIN_STATEMENT = true;
		STATES.push_back(new function_expression(*func_state));
		return "";
	}
	if (if_expression* if_state = dynamic_cast<if_expression*>(exp)) {
		if (if_state->get_lines().size() != 0) {
			if_state->execute();
			return "";
		}
		else {
			WITHNIN_STATEMENT = true;
			STATES.push_back(new if_expression(*if_state));
			return "";
		}
		
	}
	else if (while_loop_expression* w = dynamic_cast<while_loop_expression*>(exp)) {
		if (w->get_lines().size() != 0) {
			w->execute();
			return "";
		}
		else {
			WITHNIN_STATEMENT = true;
			STATES.push_back(new while_loop_expression(*w));
			return "";
		}
	}
	if (litteral_expression* n = dynamic_cast<litteral_expression*>(exp)) {
		
		if (first && (/*WITHNIN_FILE ||*/ WITHNIN_STATEMENT)) {
			exit = true;
			bag->add_error(new error("SyntaxError at line " + std::to_string(CURRENT_LINE) + " '"+exp->to_string() + "'\n"));
			return "";
		}
		
		token tok = n->get_token();
		std::string val = tok.get_text();
		if (tok.get_kind() == IDENTIFIER_TOKEN) {
			if (CURRENT_MEM->has_var(tok.get_text())) {
				variable* var = CURRENT_MEM->get_var(tok.get_text());
				val = var->get_value();
				if (var->get_type() == STRING_VAR && val[0] != syntax::QUOTES[0]) {
					val =  syntax::QUOTES + val + syntax::QUOTES;
				}
			}
			else
			{
				delete n;
				exit = true;
				bag->add_error(new error("Undefined identifier \"" + tok.get_text() + "\""));
				return "";
			}
		}
		if (variable::typeof(val) == FLOAT_VAR) {
			while (val[val.length() - 1] == '0') {
				val = val.substr(0, val.length() - 1);
			}
			if (val[val.length() - 1] == '.') {
				val = val.substr(0, val.length() - 1);
			}
		}

		return val;
	}
	else if (binary_expression* b = dynamic_cast<binary_expression*>(exp)) {
		std::string left = evaluate_expression(b->get_left(),false);
		std::string right = evaluate_expression(b->get_right(), false);

		if (exit) { return ""; }
		types left_type = variable::typeof(left);
		types right_type = variable::typeof(right);
		if (left_type == STRING_VAR && right_type == STRING_VAR) {
			if (b->get_op().get_kind() != PLUS_TOKEN) {
				bag->add_error(new error("Operator '" + b->get_op().get_text() + "' is not supported between '" + types_to_string[left_type] + "' and '" + types_to_string[right_type] + "'"));
				exit = true;
				return "";
			}

			return syntax::QUOTES + left.substr(1,left.size() - 2) + right.substr(1, right.size() - 2) + syntax::QUOTES;
		}
		if (is_bitwise(b->get_op().get_kind())) {
			if (left_type != INTEGER || right_type != INTEGER) {
				bag->add_error(new error("Operator '" + b->get_op().get_text() + "' is not supported between '" + types_to_string[left_type] + "' and '" + types_to_string[right_type] + "'"));
				exit = true;
				return "";
			}
		}
		if ((left_type != INTEGER && left_type != FLOAT_VAR) || (right_type != INTEGER && right_type != FLOAT_VAR))
		{
			bag->add_error(new error("Operator '" + b->get_op().get_text() + "' is not supported between '" + types_to_string[left_type] + "' and '" + types_to_string[right_type] + "'"));
			exit = true;
			return "";
		}
		else if (left_type == FLOAT_VAR || right_type == FLOAT_VAR) {
			float _left = std::stof(left);
			float _right = std::stof(right);
			syntax_kind k = b->get_op().get_kind();
			std::string res =  evaluate_binary(k, _left, _right);

			if (variable::typeof(res) == FLOAT_VAR) {
				for (int i = res.size() - 1; i >= 0; --i) {
					if (res[i] != '0') {
						break;
					}
					res = res.substr(0, res.size() - 1);
				}
			}
			if (res[res.size() - 1] == '.') res = res.substr(0, res.size() - 1);
			return res;

		}
		else {
			int _left = std::stoi(left);
			int _right = std::stoi(right);

			std::string res =  evaluate_binary(b->get_op().get_kind(), _left, _right);
			
			if (variable::typeof(res) == FLOAT_VAR) {
				for (int i = res.size() - 1; i >= 0; --i) {
					if (res[i] != '0') {
						break;
					}
					res = res.substr(0, res.size() - 1);
				}
			}
			if(res[res.size() - 1] == '.') res = res.substr(0, res.size() - 1);
			return res;
		}


		

		

	}
	else if (boolean_expression* b = dynamic_cast<boolean_expression*>(exp)) {
		std::string left = evaluate_expression(b->get_left(), false);
		std::string right = evaluate_expression(b->get_right(), false);
		if (exit) {
			return "";
		}
		if (b->get_op().get_kind() != BOOLEAN_GREATER && b->get_op().get_kind() != BOOLEAN_SMALLER && b->get_op().get_kind() != BOOLEAN_EQUALS_TOKEN && b->get_op().get_kind() != BOOLEAN_NOT_EQUALS_TOKEN) {
			if (variable::typeof(left) != BOOLEAN_VAR || variable::typeof(right) != BOOLEAN_VAR) {
				bag->add_error(new error("Operator '" + b->get_op().get_text() + "' is not supported between '" + types_to_string[variable::typeof(left)] + "' and '" + types_to_string[variable::typeof(right)] + "'"));
				exit = true;
				return "";
			}
		}
		else if (b->get_op().get_kind() == BOOLEAN_NOT_EQUALS_TOKEN || b->get_op().get_kind() == BOOLEAN_EQUALS_TOKEN) {
			if (variable::typeof(left) != variable::typeof(right)) {
				types left_type = variable::typeof(left);
				types right_type = variable::typeof(right);
				if ((left_type != INTEGER && left_type != FLOAT_VAR) || (right_type != INTEGER && right_type != FLOAT_VAR)) {
					bag->add_error(new error("Operator '" + b->get_op().get_text() + "' is not supported between '" + types_to_string[variable::typeof(left)] + "' and '" + types_to_string[variable::typeof(right)] + "'"));
					exit = true;
					return "";
				}
			}
		}
		else if (b->get_op().get_kind() == BOOLEAN_GREATER && b->get_op().get_kind() == BOOLEAN_SMALLER) {
			types left_type = variable::typeof(left);
			types right_type = variable::typeof(right);
			if ((left_type != INTEGER && left_type != FLOAT_VAR) || (right_type != INTEGER && right_type != FLOAT_VAR)) {
				bag->add_error(new error("Operator '" + b->get_op().get_text() + "' is not supported between '" + types_to_string[variable::typeof(left)] + "' and '" + types_to_string[variable::typeof(right)] + "'"));
				exit = true;
				return "";
			}
		}
		switch (b->get_op().get_kind()) {
			case BOOLEAN_EQUALS_TOKEN:
				if (right == left) {
					return syntax::TRUE_KEYWORD;
				}
				return syntax::FALSE_KEYWORD;
			case BOOLEAN_NOT_EQUALS_TOKEN:
				if (left != right) {
					return syntax::TRUE_KEYWORD;
				}
				return syntax::FALSE_KEYWORD;

			case BOOLEAN_AND_TOKEN:
				if (to_boolean(left) && to_boolean(right)) {
					return syntax::TRUE_KEYWORD;
				}
				return syntax::FALSE_KEYWORD;
			case BOOLEAN_OR_TOKEN:
				if (to_boolean(left) || to_boolean(right)) {
					return syntax::TRUE_KEYWORD;
				}
				return syntax::FALSE_KEYWORD;
			case BOOLEAN_GREATER:
				if (!is_a_number(left) || !is_a_number(right)) {
					bag->add_error(new error("Operator '" + b->get_op().get_text() + "' is not supported between '" + types_to_string[variable::typeof(left)] + "' and '" + types_to_string[variable::typeof(right)] + "'"));
					exit = true;
					return "";
				}
				else {
					return (std::stof(left) > std::stof(right)) ? syntax::TRUE_KEYWORD : syntax::FALSE_KEYWORD;
				}
			case BOOLEAN_SMALLER:
				if (!is_a_number(left) || !is_a_number(right)) {
					bag->add_error(new error("Operator '" + b->get_op().get_text() + "' is not supported between '" + types_to_string[variable::typeof(left)] + "' and '" + types_to_string[variable::typeof(right)] + "'"));
					exit = true;
					return "";
				}
				else {
					return (std::stof(left) < std::stof(right)) ? syntax::TRUE_KEYWORD : syntax::FALSE_KEYWORD;
				}
		}
	}
	else if (assignment_expression* a = dynamic_cast<assignment_expression*>(exp)) {
		std::string var_name = a->get_identifier()->to_string();
		std::string var_value = evaluate_expression(a->get_value_expression(), false);
		if (exit) {
			return "";
		}
		if (CURRENT_MEM->has_var(var_name)) {
			variable* var = CURRENT_MEM->get_var(var_name);
			var->change_value(var_value);
		}
		else {
			variable::create(var_name, var_value);
		}
		return "";
	}
	else if (parenthesize_expression* p = dynamic_cast<parenthesize_expression*>(exp)) {
		return evaluate_expression(p->get_expression(),false);
	}
}

std::string evaluator::evaluate(syntax_tree* tree, bool first) {
	return evaluate_expression(tree->get_root(),first);
}

bool evaluator::has_errors() { return bag->size() > 0; }
error* evaluator::get_error(int idx) { return bag->get_error(idx); }
evaluator::~evaluator() { delete bag; }