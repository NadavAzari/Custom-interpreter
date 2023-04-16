#pragma once
#include <iostream>
#include "type.h"

class variable {
private:
	std::string name;
	std::string value;
	types type;
	variable(std::string, types, std::string);
public:
	static bool is_valid_name(std::string);
	static void create(std::string, std::string);
	static types typeof(std::string);
	types get_type();
	std::string get_name();
	std::string get_value();
	void change_value(std::string);
};