#pragma once
#include <iostream>
#include <vector>
#include "expressions.h"
#include "memory.h"

class Memory;

extern bool WITHNIN_FILE;
extern bool WITHNIN_STATEMENT;
extern Memory* CURRENT_MEM;
extern int CURRENT_LINE;
extern std::vector<statement_expression*> STATES;
extern bool IN_FUNCTION;