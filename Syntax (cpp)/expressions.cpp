#include "expressions.h"
#include "evaluator.h"
#include "global.h"

#pragma region LITTERAL_EXPRESSION
token litteral_expression::get_token() { return _token; };
litteral_expression::litteral_expression(token tk) : _token(tk) {};
#pragma endregion

#pragma region BINARY_EXPRESSION
binary_expression::binary_expression(expression* _left, token _op, expression* _right) : left(_left), op(_op), right(_right) {};
expression* binary_expression::get_left() { return left; }
token binary_expression::get_op() { return op; }
expression* binary_expression::get_right() { return right; }
binary_expression::~binary_expression() {
	delete left;
	delete right;
}
#pragma endregion

#pragma region BOOLEAN_EXPRESSION

boolean_expression::boolean_expression(expression* _left, token _op, expression* _right) : left(_left), op(_op), right(_right) {};
expression* boolean_expression::get_left() { return left; }
token boolean_expression::get_op() { return op; }
expression* boolean_expression::get_right() { return right; }
boolean_expression::~boolean_expression() {
	delete left;
	delete right;
}


#pragma endregion

#pragma region PARENTHESIZE_EXPRESSION
parenthesize_expression::parenthesize_expression(token o, expression* e, token c) : _open(o), _expression(e), _close(c) {}
parenthesize_expression::~parenthesize_expression() {
	delete _expression;
}
token parenthesize_expression::get_open_paren() { return _open; }
expression* parenthesize_expression::get_expression() { return _expression; }
token parenthesize_expression::get_close_paren() { return _close; }
#pragma endregion

#pragma region ASSIGNMENT_EXPRESSION
assignment_expression::assignment_expression(expression* _identifier, token _et, expression* exp) : identifier(_identifier), equals_token(_et), value(exp) {}
assignment_expression::~assignment_expression() { delete value; }
expression* assignment_expression::get_identifier() { return identifier; }
expression* assignment_expression::get_value_expression() { return value; }
#pragma endregion

#pragma region IF_EXPRESSION
if_expression::if_expression(boolean_expression* _condition) : condition(_condition) {}
if_expression::~if_expression() { delete condition; }

std::string if_expression::execute() {

	evaluator e;
	syntax_tree* tree = new syntax_tree(condition, new errors_bag);
	std::string result = e.evaluate(tree,false);
	WITHNIN_STATEMENT = true;

	if (result == syntax::TRUE_KEYWORD) {
		for (int i = 0; i < lines.size(); ++i) {
			if (lines[i]->get_root()->get_kind() == RETURN_EXPRESSION) {
				return_expression* exp = dynamic_cast<return_expression*>(lines[i]->get_root());
				syntax_tree* t = new syntax_tree(exp->get_exp(), new errors_bag);
				WITHNIN_STATEMENT = false;
				auto v =  e.evaluate(t,false);
				WITHNIN_STATEMENT = true;

				if (e.has_errors()) {
					std::cout << e.get_error(0)->get_message();
					WITHNIN_STATEMENT = false;
					return "";
				}
				WITHNIN_STATEMENT = false;
				return v;
			}
			else if (lines[i]->get_root()->get_kind() == IF_EXPRESSION) {
				std::string val = dynamic_cast<if_expression*>(lines[i]->get_root())->execute();
				if (val != "") {
					WITHNIN_STATEMENT = STATES.size() > 0;
					return val;
				}
			}
			e.evaluate(lines[i],false);
			if (e.has_errors()) {
				std::cout << e.get_error(0)->get_message();
				WITHNIN_STATEMENT = false;
				return "";
			}
		}
	}
	WITHNIN_STATEMENT = STATES.size() > 0;
	return "";
}
#pragma endregion

#pragma region FUNCTION_EXPRESSION
function_expression::function_expression(token tok, std::vector<std::string> par) : func_identifier(tok), params_names(par) {};
void function_expression::add_param(std::string param) { params_names.push_back(param); }
token function_expression::get_identifier() { return func_identifier; }
std::vector<std::string> function_expression::get_params() { return params_names; }
#pragma endregion

#pragma region FUNCTION_CALL_EXPRESSION
function_call_expression::function_call_expression(token ident, std::vector<syntax_tree*> par) : identifier(ident), params(par) {};
token function_call_expression::get_identifier() { return identifier; }
void function_call_expression::add_param(syntax_tree* param) { params.push_back(param); }
std::vector<syntax_tree*> function_call_expression::get_params() { return params; }
#pragma endregion

return_expression::return_expression(token t , expression* e) : returnTok(t), exp(e) {}
expression* return_expression::get_exp() { return exp; }

while_loop_expression::while_loop_expression(boolean_expression* b) : condition(b) {};
while_loop_expression::~while_loop_expression() { delete condition; }
//boolean_expression* while_loop_expression::get_condition() { return condition; }
std::string while_loop_expression::execute() {
	evaluator e;
	syntax_tree* tree = new syntax_tree(condition, new errors_bag);
	std::string result = e.evaluate(tree);
	WITHNIN_STATEMENT = true;

	while (result == syntax::TRUE_KEYWORD) {
		for (int i = 0; i < lines.size(); ++i) {
			if (lines[i]->get_root()->get_kind() == RETURN_EXPRESSION) {
				return_expression* exp = dynamic_cast<return_expression*>(lines[i]->get_root());
				syntax_tree* t = new syntax_tree(exp->get_exp(), new errors_bag);
				WITHNIN_STATEMENT = false;
				auto v = e.evaluate(t,false);
				WITHNIN_STATEMENT = true;

				if (e.has_errors()) {
					std::cout << e.get_error(0)->get_message();
					WITHNIN_STATEMENT = false;
					return "";
				}
				WITHNIN_STATEMENT = false;
				return v;
			}
			else if (lines[i]->get_root()->get_kind() == IF_EXPRESSION) {
				std::string val = dynamic_cast<if_expression*>(lines[i]->get_root())->execute();
				if (val != "") {
					return val;
				}
			}
			else {
				e.evaluate(lines[i], false);
				if (e.has_errors()) {
					std::cout << e.get_error(0)->get_message();
					WITHNIN_STATEMENT = false;
					return "";
				}
			}

		}
		result = e.evaluate(tree,false);
	}
	WITHNIN_STATEMENT = STATES.size() > 0;
	return "";
}