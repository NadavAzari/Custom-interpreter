#pragma once
#include <iostream>
#include <string>
#include <vector>

class syntax {
public:

	static void initialize();


	static const std::string PLUS;
	static const std::string MINUS;
	static const std::string STAR;
	static const std::string SLASH;
	static const std::string POWER;
	static const std::string XOR;
	static const std::string AND;
	static const std::string OR;


	static const std::string BOOLEAN_NOT;
	static const std::string BOOLEAN_EQUALS;
	static const std::string BOOLEAN_NOT_EQUALS;
	static const std::string BOOLEAN_AND;
	static const std::string BOOLEAN_OR;
	static const std::string BOOLEAN_GREATER;
	static const std::string BOOLEAN_SMALLER;

	static const std::string EQUALS;


	static const std::string ENTER_STATEMENT;
	static const std::string LEAVE_STATEMENT;

	static const std::string OPEN_PAREN;
	static const std::string CLOSE_PAREN;
	static const std::string COMMA;
	static const std::string FUNCTION_KEYWORD;
	static const std::string WHILE_LOOP_KEYWORD;

	static const std::string TRUE_KEYWORD;
	static const std::string FALSE_KEYWORD;
	static const std::string IF_KEYWORD;
	static const std::string RETURN_KEYWORD;

	static const std::string PRINT_FUNC;
	static const std::string RAND_FUNC;
	static const std::string INPUT_FUNC;
	static const std::string TO_NUM_FUNC;
	static const std::string TO_STRING_FUNC;
	static const std::string IS_EXIST;
	static const std::string EVAL;

	static const std::string EQUALS;

	static const std::vector<std::string> KEYWORDS;
	static std::vector<std::string> OPERATORS;

	static const std::string QUOTES;

private:
	static bool compare_length(std::string s1, std::string s2);
};