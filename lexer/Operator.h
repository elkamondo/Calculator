#ifndef OPERATOR_H
#define OPERATOR_H

#include <stdbool.h>
#include "Token.h"

/**
 * @brief Represents the type of the operator associativity
 */
typedef enum associativity_type { LEFT, RIGHT } AssocType;

/**
 * @brief Represents the operator type which holds the operator information
 */
typedef struct operator_t
{
  unsigned int precedence;
  AssocType associativity;
  char value[2];
} operator_t, *Operator;

/**
 * @brief Creates an operator type
 */
Operator create_operator(TokenType, const char*);

/**
 * @brief Returns a copy of a given operator type
 */
Operator clone_operator(Operator);

/**
 * @brief Checks a given token type, represents an operator
 */
bool is_operator(TokenType);

/**
 * @brief Prints a given operator type
 */
void print_operator(Operator);

/**
 * @brief Evaluates an operator calculation
 */
long double eval_operator(TokenType, long double, long double);

#endif