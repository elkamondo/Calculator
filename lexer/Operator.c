#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "../CommonHeaders.h"
#include "Operator.h"
#include "Token.h"


/**
 * @brief Creates an operator type
 * @details Associate to each operator their propreties:
 *          ----------------------------------------
 *         | Operator | Precedence | Associativity |
 *         -----------------------------------------
 *         |     ^    |      4     |   Right       |
 *         |     *    |      3     |   Left        |
 *         |     /    |      3     |   Left        |
 *         |     %    |      3     |   Left        |
 *         |     +    |      2     |   Left        |
 *         |     -    |      2     |   Left        |
 *         -----------------------------------------
 *
 * @param type The token type
 * @param value The operator value
 *
 * @return The address of the created operator type
 */
Operator create_operator(TokenType type, const char *value)
{
  Operator operator = malloc(sizeof(*operator));
  assert(operator != NULL);

  operator->precedence    = 0;
  operator->associativity = 0;
  strcpy(operator->value, value);

  operator_t ops[] = {
    {4U, RIGHT, "^"}, {3U, LEFT, "*"}, {3U, LEFT, "/"},
    {3U, LEFT, "%"},  {2U, LEFT, "+"}
  };
  const size_t nbr_operators = sizeof ops / sizeof *ops;

  if (type == UMINUS)
    operator->precedence = 4U;
  else if (type == BMINUS)
    operator->precedence = 2U;

  if (type == MINUS) {
    operator->associativity = LEFT;
  } else {
    for (size_t i = 0; i < nbr_operators; ++i) {
      if (ops[i].value[0] == value[0]) {
        operator->associativity = ops[i].associativity;
        operator->precedence    = ops[i].precedence;
      }
    }
  }
  return operator;
}


/**
 * @brief Returns a copy of a given operator type
 *
 * @param operator The operator to clone
 * @return The copy of the operator
 */
Operator clone_operator(Operator operator)
{
  Operator copy = malloc(sizeof(*copy));
  assert(copy != NULL);

  copy->precedence    = operator->precedence;
  copy->associativity = operator->associativity;
  strcpy(copy->value, operator->value);

  return copy;
}


/**
 * @brief Checks a given token type, represents an operator
 *
 * @param type The type of the token
 * @return True if the type represents an operator, false otherwise
 */
bool is_operator(TokenType type)
{
  return type == EXPONENT
      || type == MULTIPLY
      || type == DIVIDE
      || type == PLUS
      || type == MINUS
      || type == UMINUS
      || type == BMINUS
      || type == MODULO;
}


/**
 * @brief Prints a given operator type
 *
 * @param operator The operator to print
 */
void print_operator(Operator operator)
{
  printf(" %s ", operator->value);
}


/**
 * @brief Evaluates an operator calculation
 *
 * @param type The type of the operator
 * @param lc The first operand
 * @param rc The second operand
 * @return The result of calcul
 */
long double eval_operator(TokenType type, long double lc, long double rc)
{
  long double result = 0.0;
  switch (type) {
    case PLUS:
                result = lc + rc;
                break;
    case BMINUS:
                result = lc - rc;
                break;
    case UMINUS:
                result = -rc;
                break;
    case MULTIPLY:
                result = lc * rc;
                break;
    case EXPONENT:
                result = powl(lc, rc);
                break;
    case MODULO:
                result = remainderl(lc, rc);
                break;
    case DIVIDE:
                result = lc / rc;
                break;
    default:
                break;
  }

  return result;
}