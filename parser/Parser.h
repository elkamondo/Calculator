#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "../lexer/List.h"

/**
 * @brief Creates a parse tree from a list of tokens
 */
ASTNode parse_expression(List);

#endif