#pragma once
#include "variable.h"
#include <map>
#include "expressions.h"
#include "function.h"

class Memory;
class function;

class Memory {
private:
	static Memory* main_mem;
	std::map<std::string, variable*> variables;
	std::map<std::string, function*> functions;
public:
	static Memory* main();
	static void initialize();
	static Memory* create();

	void add_var(variable* var);
	bool has_var(std::string _name);
	variable* get_var(std::string _name);

	void clear_vars();
	
	void add_function(std::string , function* );
	bool has_function(std::string _name);
	function* get_function(std::string _name);
};