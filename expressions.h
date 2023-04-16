#pragma once
#include "token.h"
#include "syntax.h"
#include "syntax_tree.h"


class syntax_tree;

enum expression_kind {
	LITTERAL_EXPRESSION,
	BINARY_EXPRESSION,
	PARENTHESIZE_EXPRESSION,
	BOOLEAN_EXPRESSION,
	ASSINGMENT_EXPRESSION,
	STRING_EXPRESSION,
	IF_EXPRESSION,
	FUNCTION_EXPRESSION,
	FUNCTION_CALL_EXPRESSION,
	RETURN_EXPRESSION,
	WHILE_EXPRESSION,
};

class expression {
public:
	virtual expression_kind get_kind() = 0;
	virtual std::string to_string() = 0;
};

class litteral_expression : public expression{
private:
	token _token;
public:
	litteral_expression(token);
	token get_token();
	expression_kind get_kind() override {
		return LITTERAL_EXPRESSION;
	}
	std::string to_string() override {
		return _token.get_text();
	}
};

class binary_expression : public expression {
private:
	expression* left;
	expression* right;
	token op;
public:
	binary_expression(expression*, token, expression*);
	~binary_expression();
	expression* get_left();
	token get_op();
	expression* get_right();
	expression_kind get_kind() override {
		return BINARY_EXPRESSION;
	}
	std::string to_string() override {
		return left->to_string() + op.get_text() + right->to_string();
	}
};

class boolean_expression : public expression{
private:
	expression* left;
	expression* right;
	token op;
public:
	boolean_expression(expression*, token, expression*);
	~boolean_expression();
	expression* get_left();
	token get_op();
	expression* get_right();
	expression_kind get_kind() override {
		return BOOLEAN_EXPRESSION;
	}

	std::string to_string() override {
		return left->to_string() + op.get_text() + right->to_string();
	}

};

class parenthesize_expression : public expression {
private:
	expression* _expression;
	token _open, _close;
public:
	parenthesize_expression(token, expression*, token);
	~parenthesize_expression();
	expression_kind get_kind() override {
		return PARENTHESIZE_EXPRESSION;
	};
	token get_open_paren();
	expression* get_expression();
	token get_close_paren();
	std::string to_string() override {
		return _open.get_text() + _expression->to_string() + _close.get_text();
	}
};

class assignment_expression : public expression {
private:
	expression* identifier;
	token equals_token;
	expression* value;
public:
	assignment_expression(expression*, token, expression*);
	~assignment_expression();
	expression_kind get_kind() override {
		return ASSINGMENT_EXPRESSION;
	};
	expression* get_identifier();
	expression* get_value_expression();
	std::string to_string() override {
		return identifier->to_string() + equals_token.get_text() + value->to_string();
	}
};

class statement_expression : public expression {
protected:
	std::vector<syntax_tree*> lines;
public:
	virtual std::string to_string() = 0;
	virtual std::string execute() = 0;
	void add_line(syntax_tree* line) { lines.push_back(line); }
	std::vector<syntax_tree*> get_lines() { return lines; }
	~statement_expression() {
		for (int i = 0; i < lines.size(); ++i) {
			delete lines[i];
		}
	}
	
};

class if_expression : public statement_expression {
private: 
	boolean_expression* condition;
public:

	std::string to_string() override {
		return "";
	};
	expression_kind get_kind() override {
		return IF_EXPRESSION;
	};
	boolean_expression* get_condition() { return condition; };
	std::string execute() override;
	if_expression(boolean_expression*);
	~if_expression();
};

class function_expression : public statement_expression {
private:
	token func_identifier;
	std::vector<std::string> params_names;
public:
	expression_kind get_kind() override {
		return FUNCTION_EXPRESSION;
	};

	std::string to_string() override {
		return "";
	};

	std::string execute() override {
		return "";
	};

	function_expression(token, std::vector<std::string>);

	token get_identifier();

	std::vector<std::string> get_params();
	void add_param(std::string);

};

class function_call_expression : public expression {
private:
	token identifier;
	std::vector<syntax_tree*> params;

	expression_kind get_kind() override {
		return FUNCTION_CALL_EXPRESSION;
	};

	std::string to_string() override {
		return "";
	};

public:
	function_call_expression(token, std::vector<syntax_tree*>);
	~function_call_expression() {
		for (int i = 0; i < params.size(); ++i) { delete params[i]; }
	}
	void add_param(syntax_tree*);
	token get_identifier();
	std::vector<syntax_tree*> get_params();

	//ILL DELETE THE SYNTAX TREES IN ANOTHER CODE STATEMENT
};

class return_expression : public expression {
private:
	token returnTok;
	expression* exp;
public:
	return_expression(token, expression*);
	expression_kind get_kind() override {
		return RETURN_EXPRESSION;
	};
	std::string to_string() override {
		return returnTok.get_text() + exp->to_string();
	};
	~return_expression() { delete exp; }
	expression* get_exp();

};

class while_loop_expression : public statement_expression {
private:
	boolean_expression* condition;
public:
	std::string to_string() override {
		return "";
	};
	expression_kind get_kind() override {
		return WHILE_EXPRESSION;
	};
	boolean_expression* get_condition() { return condition; };
	std::string execute() override;
	while_loop_expression(boolean_expression*);
	~while_loop_expression();
};