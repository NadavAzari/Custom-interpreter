#include "parser.h"
#include "expressions.h"
#include <map>
#include "global.h"
#include "evaluator.h"

class function_expression;

std::map<syntax_kind, std::string> kinds = {
    {PLUS_TOKEN, "PlusToken"},
    {MINUS_TOKEN,"MinusToken"},
    {STAR_TOKEN, "StarToken"},
    {SLASH_TOKEN, "SlashToken"},
    {POWER_TOKEN, "PowerToken" },
    {NUMBER_TOKEN, "NumberToken"},
    {OPEN_PAREN_TOKEN, "OpenParenToken"},
    {CLOSE_PAREN_TOKEN, "CloseParenToken"},
    {END_OF_FILE_TOKEN, "EndOfFileToken"},
    {ENTER_STATEMENT, "EnterStatement"},
    {LEAVE_STATEMENT, "LeaveStatement"},
    {BAD_TOKEN, "BadToken"},
    {IDENTIFIER_TOKEN, "IdentifierToken"}
};

bool is_boolean_operator(syntax_kind kind) {
    if (kind == BOOLEAN_AND_TOKEN || kind == BOOLEAN_EQUALS_TOKEN
        || kind == BOOLEAN_NOT_TOKEN || kind == BOOLEAN_NOT_EQUALS_TOKEN
        || kind == BOOLEAN_OR_TOKEN || kind == BOOLEAN_GREATER || kind == BOOLEAN_SMALLER) {
        return true;
    }
    return false;
}

int get_prec(syntax_kind op)
{
    switch (op)
    {
    case POWER_TOKEN:
        return 7;
    case STAR_TOKEN:
    case SLASH_TOKEN:
        return 6;
    case PLUS_TOKEN:
    case MINUS_TOKEN:
    case XOR_TOKEN: 
    case AND_TOKEN:
    case OR_TOKEN:
        return 5;
    
    case BOOLEAN_EQUALS_TOKEN:
    case BOOLEAN_GREATER:
    case BOOLEAN_SMALLER:
    case BOOLEAN_NOT_EQUALS_TOKEN:
        return 4;
    
    //case BitwiseAndToken:
    case BOOLEAN_AND_TOKEN:
        return 3;

    //case BitwiseOrToken:
    case BOOLEAN_OR_TOKEN:
    case BOOLEAN_NOT_TOKEN:
        return 2;
    case EQUALS_TOKEN:
        return 1;
    

    default:
        return 0;
    }
}

parser::parser(std::string line) : pos(0) {
    lexer _lexer(line);

    while (true) {
        token _token = _lexer.next();
        if (_token.get_kind() != SPACE_TOKEN) {
            tokens.push_back(_token);
        }
        if (_token.get_kind() == END_OF_FILE_TOKEN) {
            break;
        }
    }
    bag = _lexer.bag;
}

token parser::match(syntax_kind _kind) {
    if (tokens[pos].get_kind() == _kind) {
        return tokens[pos++];
    }
    bag->add_error(new error("Line "+ std::to_string(CURRENT_LINE)+ " Unexpected token <" + kinds[tokens[pos].get_kind()] + ">, expected <" + kinds[_kind] + ">"));
    return token(pos, "\0", _kind);
}

expression* parser::parse_primary() {
    if (tokens[pos].get_kind() == RETURN_KEYWORD) {
        token tok = tokens[pos++];
        expression* exp = parse_expression();
        return new return_expression(tok, exp);
    }
    if (tokens[pos].get_kind() == IF_KEYWORD) {
        pos++;
        expression* exp = parse_expression();
       // evaluator ev;
        //auto res = ev.evaluate(new syntax_tree(exp, new errors_bag), new errors_bag);
        if (exp->get_kind() != BOOLEAN_EXPRESSION){
            bag->add_error(new error("Line " + std::to_string(CURRENT_LINE) + " if statement must followed by boolean expression"));
            return exp;
        }
        if_expression* if_state = new if_expression((boolean_expression*)exp);
        match(ENTER_STATEMENT);
        return if_state;

    }
    else if (tokens[pos].get_kind() == WHILE_KEYWORD) {
        pos++;
        expression* exp = parse_expression();
        //evaluator ev;
        //auto res = ev.evaluate(new syntax_tree(exp, new errors_bag), new errors_bag);
        if (exp->get_kind() != BOOLEAN_EXPRESSION) {
            bag->add_error(new error("Line " + std::to_string(CURRENT_LINE) + " while statement must followed by boolean expression"));
            return exp;
        }
        while_loop_expression* while_loop = new while_loop_expression((boolean_expression*)exp);
        match(ENTER_STATEMENT);
        return while_loop;
    }
    if (tokens[pos].get_kind() == FUNCTION_KEYWORD) {
        pos++;
        return new function_expression(match(IDENTIFIER_TOKEN), std::vector<std::string>());
    }
    if (tokens[pos].get_kind() == IDENTIFIER_TOKEN && peek(1).get_kind() == OPEN_PAREN_TOKEN) {
        return new function_call_expression(tokens[pos++], std::vector<syntax_tree*>());
    }
    if (tokens[pos].get_kind() == OPEN_PAREN_TOKEN) {
        token _open = tokens[pos++];
        expression* exp = parse_expression();
        token _close = match(CLOSE_PAREN_TOKEN);
        return new parenthesize_expression(_open, exp, _close);
    }
    else if (tokens[pos].get_kind() == TRUE_KEYWORD || tokens[pos].get_kind() == FALSE_KEYWORD) {
        return new litteral_expression(tokens[pos++]);
    }
    else if (tokens[pos].get_kind() == IDENTIFIER_TOKEN || tokens[pos].get_kind() == STRING_TOKEN) {
        return new litteral_expression(tokens[pos++]);
    }
    litteral_expression* exp = new litteral_expression(match(NUMBER_TOKEN));
    return exp;
}

expression* parser::parse_expression(int lastPrec) {
    expression* left = parse_primary();
    if (left->get_kind() == FUNCTION_EXPRESSION) {
        match(OPEN_PAREN_TOKEN);


        while (true) {
            if (tokens[pos].get_kind() == IDENTIFIER_TOKEN) {
                token current_param = match(IDENTIFIER_TOKEN);
                function_expression* f = dynamic_cast<function_expression*>(left);
                f->add_param(current_param.get_text());
                
                if(tokens[pos].get_kind() != COMMA_TOKEN) {
                    match(CLOSE_PAREN_TOKEN);
                    match(ENTER_STATEMENT);
                    return left;
                }
                pos++;
            }
            else {
                match(CLOSE_PAREN_TOKEN);
                match(ENTER_STATEMENT);
                return left;
            }

        }
    }
    else if (left->get_kind() == FUNCTION_CALL_EXPRESSION) {
        match(OPEN_PAREN_TOKEN);
        function_call_expression* f = dynamic_cast<function_call_expression*>(left);
        while (true) {
            if (tokens[pos].get_kind() == CLOSE_PAREN_TOKEN) {
                return left;
            }
            expression* current_param = parse_expression();
            syntax_tree* tree = new syntax_tree(current_param, new errors_bag);
            f->add_param(tree);
            if (tokens[pos].get_kind() == COMMA_TOKEN) { pos++; }
            else {
                match(CLOSE_PAREN_TOKEN);
                return left;
            }
        }
    }
    while (true) {
        int prec = get_prec(tokens[pos].get_kind());
        if (prec == 0 || prec <= lastPrec)
            break;
        token _operator = tokens[pos++];
        expression* right = parse_expression(prec);
        if (_operator.get_kind() == EQUALS_TOKEN) {
            left = new assignment_expression(left, _operator, right);
        }
        else if (is_boolean_operator(_operator.get_kind())) {
            left = new boolean_expression(left, _operator, right);
        }
        else {
            left = new binary_expression(left, _operator, right);
        }

    }
    return left;
}

syntax_tree* parser::parse() {
    expression* exp = parse_expression();
    return new syntax_tree(exp,bag);
}

token parser::peek(int offset) {
    if (pos + offset >= tokens.size()) {
        return tokens[tokens.size() - 1];
    }
    return tokens[pos + offset];
}