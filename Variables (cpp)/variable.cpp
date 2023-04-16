#include "variable.h"
#include "type.h"
#include "memory.h"
#include "syntax.h"
#include "global.h"

bool is_number(std::string text) {
	int c = 0;
	for (int i = 0; i < text.length(); ++i) {
		if (!std::isdigit(text[i]) && text[i] != '.')
			return false;
		if (text[i] == '.') c++;
		if (c > 1) {
			return false;
		}
	}
	return true;
}

bool is_int_num(std::string text) {
	return is_number(text) && text.find(".") == std::string::npos;
}

variable::variable(std::string _name, types _type, std::string _value) : name(_name), type(_type), value(_value) {
	if (type == STRING_VAR) {
		value = value.substr(1, value.size() - 2);
	}
}

bool variable::is_valid_name(std::string _name) {
	return true;
}

types variable::get_type() {
	return type;
}

std::string variable::get_name() {
	return name;
}

void variable::create(std::string var_name, std::string var_value) {
	variable* var = new variable(var_name, variable::typeof(var_value), var_value);
	CURRENT_MEM->add_var(var);
}

std::string variable::get_value() {
	return value;
}

void variable::change_value(std::string val) {
	type = variable::typeof(val);
	value = val;
	if (type == STRING_VAR) {
		value =  value.substr(1, value.size() - 2);
	}
}

types variable::typeof(std::string value) {
	if (value == syntax::TRUE_KEYWORD || value == syntax::FALSE_KEYWORD) {
		return BOOLEAN_VAR;
	}
	else if (is_int_num(value)) {
		return INTEGER;
	}
	else if (is_number(value)) {
		return FLOAT_VAR;
	}
	else {
		return STRING_VAR;
	}
}