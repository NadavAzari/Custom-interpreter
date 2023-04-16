#include "syntax.h"

//BOOLEAN OPERATORS IDENTIFIERS
const std::string syntax::BOOLEAN_NOT = "!";
const std::string syntax::BOOLEAN_EQUALS = "==";
const std::string syntax::BOOLEAN_NOT_EQUALS = "!=";
const std::string syntax::BOOLEAN_AND = "&&";
const std::string syntax::BOOLEAN_OR = "||";
const std::string syntax::BOOLEAN_GREATER = ">";
const std::string syntax::BOOLEAN_SMALLER = "<";

//MATHEMATICAL OPERATORS IDENTIFIERS
const std::string syntax::PLUS = "+";
const std::string syntax::MINUS = "-";
const std::string syntax::STAR = "*";
const std::string syntax::SLASH = "/";
const std::string syntax::POWER = "**";
const std::string syntax::XOR = "^";
const std::string syntax::AND = "&";
const std::string syntax::OR = "|";
const std::string syntax::EQUALS = "=";


const std::string syntax::ENTER_STATEMENT = "{";
const std::string syntax::LEAVE_STATEMENT = "}";


//PARENTHESIZES
const std::string syntax::OPEN_PAREN = "(";
const std::string syntax::CLOSE_PAREN = ")";
const std::string syntax::COMMA = ",";

//KEYWORDS
const std::string syntax::TRUE_KEYWORD = "true";
const std::string syntax::FALSE_KEYWORD = "false";
const std::string syntax::IF_KEYWORD = "if";
const std::string syntax::FUNCTION_KEYWORD = "function";
const std::string syntax::RETURN_KEYWORD = "return";
const std::string syntax::WHILE_LOOP_KEYWORD = "while";

const std::string syntax::PRINT_FUNC = "print";
const std::string syntax::RAND_FUNC = "rand";
const std::string syntax::INPUT_FUNC = "input";
const std::string syntax::TO_NUM_FUNC = "num";
const std::string syntax::TO_STRING_FUNC = "str";
const std::string syntax::IS_EXIST = "exists";
const std::string syntax::EVAL = "eval";

const std::string syntax::QUOTES = "\"";

std::vector<std::string> syntax::OPERATORS = {
	syntax::BOOLEAN_NOT,
	syntax::BOOLEAN_EQUALS,
	syntax::BOOLEAN_NOT_EQUALS,
	syntax::BOOLEAN_AND,
	syntax::BOOLEAN_OR,
	syntax::PLUS,
	syntax::MINUS,
	syntax::STAR,
	syntax::SLASH,
	syntax::POWER,
	syntax::EQUALS,
	syntax::XOR,
	syntax::AND,
	syntax::OR,
	syntax::BOOLEAN_GREATER,
	syntax::BOOLEAN_SMALLER,
	syntax::IF_KEYWORD,
	syntax::WHILE_LOOP_KEYWORD,
	syntax::OPEN_PAREN,
	syntax::CLOSE_PAREN,
	syntax::COMMA,
	syntax::RETURN_KEYWORD
};

const std::vector<std::string> syntax::KEYWORDS = {
	syntax::TRUE_KEYWORD,
	syntax::FALSE_KEYWORD
};

bool syntax::compare_length(std::string s1,std::string s2)
{
	return s1.length() < s2.length();
}

void syntax::initialize() {
	for (int i = 0; i < syntax::OPERATORS.size() - 1; i++)
	{
		for (int j = 0; j < syntax::OPERATORS.size() - i - 1; j++)
		{
			if (syntax::OPERATORS[j].length() > syntax::OPERATORS[j + 1].length())
			{
				// Swap strings[j] and strings[j + 1]
				std::string temp = syntax::OPERATORS[j];
				syntax::OPERATORS[j] = syntax::OPERATORS[j + 1];
				syntax::OPERATORS[j + 1] = temp;
			}
		}
	}
}
