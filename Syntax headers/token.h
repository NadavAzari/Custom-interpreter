#pragma once
#include <iostream>



enum syntax_kind {
	NUMBER_TOKEN,
	PLUS_TOKEN,
	MINUS_TOKEN,
	STAR_TOKEN,
	SLASH_TOKEN,
	POWER_TOKEN,
	XOR_TOKEN,
	AND_TOKEN,
	OR_TOKEN,

	EQUALS_TOKEN,

	ENTER_STATEMENT,
	LEAVE_STATEMENT,


	BOOLEAN_OR_TOKEN,
	BOOLEAN_AND_TOKEN,
	BOOLEAN_NOT_TOKEN,
	BOOLEAN_EQUALS_TOKEN,
	BOOLEAN_NOT_EQUALS_TOKEN,
	BOOLEAN_GREATER,
	BOOLEAN_SMALLER,
	
	OPEN_PAREN_TOKEN,
	CLOSE_PAREN_TOKEN,
	COMMA_TOKEN,

	STRING_TOKEN,

	TRUE_KEYWORD,
	FALSE_KEYWORD,
	IDENTIFIER_TOKEN,
	IF_KEYWORD,
	FUNCTION_KEYWORD,
	RETURN_KEYWORD,
	WHILE_KEYWORD,

	SPACE_TOKEN,
	END_OF_FILE_TOKEN,
	BAD_TOKEN
};


class token{
private:
	int pos;
	std::string text;
	enum syntax_kind kind;

public:
	token(int, std::string, syntax_kind);
	token() = default;
	int get_pos();
	std::string get_text();
	syntax_kind get_kind();
};