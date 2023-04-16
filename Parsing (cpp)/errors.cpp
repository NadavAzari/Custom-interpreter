#include "errors.h"

error* errors_bag::get_error(int idx) { return errors[idx]; }
void errors_bag::add_error(error* _error) { errors.push_back(_error); }
int errors_bag::size() { return errors.size(); }

error::error(std::string _message) : message(_message){}
std::string error::get_message() { return message; }