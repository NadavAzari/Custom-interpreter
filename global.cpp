#include "global.h"

class Memory;

bool WITHNIN_FILE = false;
bool WITHNIN_STATEMENT = false;
int CURRENT_LINE = 1;
Memory* CURRENT_MEM = nullptr;
std::vector<statement_expression*> STATES = std::vector< statement_expression*>();
bool IN_FUNCTION = false;