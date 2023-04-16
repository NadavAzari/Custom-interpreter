#pragma once

typedef enum {
	INTEGER,
	BOOLEAN_VAR,
	FLOAT_VAR,
	STRING_VAR,
	UNKNOWN_TYPE
} types;

class type {
public:
	virtual types get_type() = 0;
	virtual bool is_iterable() = 0;
	virtual std::string to_string() = 0;
};