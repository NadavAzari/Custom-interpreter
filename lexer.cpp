#include "lexer.h"
#include "syntax.h"
#include "variable.h"
#include <string>

bool ends_with(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
bool starts_with(std::string const& value, std::string const& start) {
	return value.rfind(start, 0) == 0;
}


lexer::lexer(std::string _line) : line(_line), pos(0), bag(new errors_bag) {}

void lexer::add_error() {
	std::string msg = "SyntaxError at position " + std::to_string(pos) + "\n";
	msg += "\t" + line + "\n\t";

	for (int i = 0; i < pos - 1; ++i) {
		msg += " ";
	}
	msg += "^";

	bag->add_error(new error(msg));
}

token lexer::next() {
	int start = pos;
	if (line.length() <= pos) {
		return token(line.length(), "\0", END_OF_FILE_TOKEN);
	}
	
	if (std::isdigit(line[pos]) || line[pos] == '.') {
		do {
			++pos;
		} while (std::isdigit(line[pos]) || line[pos] == '.');
		return token(start, line.substr(start, pos - start), NUMBER_TOKEN);
	}

	else if (line[pos] == syntax::QUOTES[0]) {
		do {
			++pos;
		} while (line[pos] != syntax::QUOTES[0]);
		pos++;
		return token(start, line.substr(start, pos - start), STRING_TOKEN);
	}
	
	else if (line[pos] == ' ' || line[pos] == '\t') {
		do {
			++pos;
		} while (line[pos] == ' ');
		return token(start, " ", SPACE_TOKEN);
	}


	else {
		int _start = pos;
		while (pos < line.length() && !std::isdigit(line[pos]) && line[pos] != ' ') {
			++pos;
		}
		std::string _text = line.substr(_start, pos - _start);
		
		/*
		else if (ends_with(_text, syntax::OPEN_PAREN)) {
			_text.replace(_text.length() - syntax::OPEN_PAREN.length(), _text.length() - 1,"");
			pos -= syntax::OPEN_PAREN.length();
		}
		
		else if (starts_with(_text, syntax::CLOSE_PAREN)) {
			_text.replace(syntax::CLOSE_PAREN.length(), _text.length(), "");
			pos = _start + syntax::CLOSE_PAREN.length();
		}
		*/
		
		for (int i = syntax::OPERATORS.size() - 1; i >=0 ; --i) {
			if (_text.find(syntax::OPERATORS[i]) != std::string::npos) {
				if (_text == syntax::OPERATORS[i])
					break;
				if (starts_with(_text, syntax::OPERATORS[i])) {
					_text.replace(syntax::OPERATORS[i].length(), _text.length(), "");
					pos = _start + syntax::OPERATORS[i].length();
					break;
				}
				int idx = _text.find(syntax::OPERATORS[i]);
				int length = _text.length();
				_text.replace(idx, length - idx, "");
				pos -= (length - idx);
			}
		}
		/*
		if (starts_with(_text, syntax::QUOTES) && ends_with(_text, syntax::QUOTES)) {
			int start = pos - _text.length();
			_text = _text.substr(syntax::QUOTES.length(), _text.length() - 2 * syntax::QUOTES.length());
			return token(start,_text, STRING_TOKEN);
		}
		*/
		
		if (_text == syntax::IF_KEYWORD) return token(_start, syntax::IF_KEYWORD, IF_KEYWORD);
		if (_text == syntax::FUNCTION_KEYWORD) return token(_start, syntax::FUNCTION_KEYWORD, FUNCTION_KEYWORD);
		if (_text == syntax::ENTER_STATEMENT) return token(_start, syntax::ENTER_STATEMENT, ENTER_STATEMENT);
		if (_text == syntax::LEAVE_STATEMENT) return token(_start, syntax::LEAVE_STATEMENT, LEAVE_STATEMENT);
		if (_text == syntax::CLOSE_PAREN) return token(_start, syntax::CLOSE_PAREN, CLOSE_PAREN_TOKEN);
		if (_text == syntax::OPEN_PAREN) return token(_start, syntax::OPEN_PAREN, OPEN_PAREN_TOKEN);
		else if (_text == syntax::PLUS) return token(_start, syntax::PLUS, PLUS_TOKEN);
		else if (_text == syntax::MINUS) return token(_start, syntax::MINUS, MINUS_TOKEN);
		else if (_text == syntax::STAR) return token(_start, syntax::STAR, STAR_TOKEN);
		else if (_text == syntax::SLASH) return token(_start, syntax::SLASH, SLASH_TOKEN);
		else if (_text == syntax::POWER) return token(_start, syntax::POWER, POWER_TOKEN);
		else if (_text == syntax::XOR) return token(_start, syntax::XOR, XOR_TOKEN);
		else if (_text == syntax::AND) return token(_start, syntax::XOR, AND_TOKEN);
		else if (_text == syntax::OR) return token(_start, syntax::XOR, OR_TOKEN);
		else if (_text == syntax::COMMA) return token(_start, syntax::COMMA, COMMA_TOKEN);

		else if (_text == syntax::BOOLEAN_OR) return token(_start, syntax::BOOLEAN_OR, BOOLEAN_OR_TOKEN);
		else if (_text == syntax::BOOLEAN_AND) return token(_start, syntax::BOOLEAN_AND, BOOLEAN_AND_TOKEN);
		else if (_text == syntax::BOOLEAN_EQUALS) return token(_start, syntax::BOOLEAN_EQUALS, BOOLEAN_EQUALS_TOKEN);
		else if (_text == syntax::BOOLEAN_NOT_EQUALS) return token(_start, syntax::BOOLEAN_NOT_EQUALS, BOOLEAN_NOT_EQUALS_TOKEN);
		else if (_text == syntax::BOOLEAN_NOT) return token(_start, syntax::BOOLEAN_NOT, BOOLEAN_NOT_TOKEN);
		else if (_text == syntax::BOOLEAN_GREATER) return token(_start, syntax::BOOLEAN_GREATER, BOOLEAN_GREATER);
		else if (_text == syntax::BOOLEAN_SMALLER) return token(_start, syntax::BOOLEAN_SMALLER, BOOLEAN_SMALLER);

		else if (_text == syntax::EQUALS) return token(_start, syntax::EQUALS, EQUALS_TOKEN);

		else if(_text == syntax::TRUE_KEYWORD) return token(_start, syntax::TRUE_KEYWORD, TRUE_KEYWORD);
		else if(_text == syntax::FALSE_KEYWORD) return token(_start, syntax::FALSE_KEYWORD, FALSE_KEYWORD);
		else if(_text == syntax::RETURN_KEYWORD) return token(_start, syntax::RETURN_KEYWORD, RETURN_KEYWORD);
		else if(_text == syntax::WHILE_LOOP_KEYWORD) return token(_start, syntax::WHILE_LOOP_KEYWORD, WHILE_KEYWORD);

		else {
			if (variable::is_valid_name(_text)) {
				return token(_start, _text, IDENTIFIER_TOKEN);
			}
		}
	}
	int tmp = pos;
	pos = start;
	add_error();
	pos = tmp;
	return token(pos, "\0", BAD_TOKEN);
}
