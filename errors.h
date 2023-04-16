#pragma once
#include <iostream>
#include <vector>

#define SYNTAX_ERROR "SyntaxError"
#define UNEXPECTED_TOKEN "Unexpected token"

class error;

class errors_bag {
private:
	std::vector<error*> errors;
public:
	void add_error(error*);
	int size();
	error* get_error(int);
};

class error {
private:
	std::string message;

public:
	error(std::string);
	std::string get_message();
};