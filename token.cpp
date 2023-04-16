#include "token.h"

token::token(int _pos, std::string _text, syntax_kind _kind) : pos(_pos), text(_text), kind(_kind) {};

int token::get_pos() { return pos; }
std::string token::get_text() { return text; }
syntax_kind token::get_kind() { return kind; }