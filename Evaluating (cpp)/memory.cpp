#include "memory.h"

Memory* Memory::main_mem = nullptr;

void Memory::initialize() {
	Memory::main_mem = new Memory();
}

Memory* Memory::create() { return new Memory(); }

variable* Memory::get_var(std::string _name) {
	return variables[_name];
}

void Memory::add_var(variable* var) {
	variables.insert({var->get_name(),var});
}

bool Memory::has_function(std::string name) {
	return functions.find(name) != functions.end();
}


function* Memory::get_function(std::string _name) {
	return functions[_name];
}

void Memory::clear_vars() {
	variables.clear();
}

void Memory::add_function(std::string name, function* func) {
	functions.insert({ name,func });
}

bool Memory::has_var(std::string name) {
	return variables.find(name) != variables.end();
}


Memory* Memory::main() {
	return main_mem;
}