#include <string.h>

#include "../CommonHeaders.h"
#include "Token.h"
#include "Operator.h"
#include "Function.h"


/**
 * @brief Prints a token
 *
 * @param token The token to print
 * @see Function::print_function, Operator::print_operator, Operator::is_operator
 */
static void print_token(Token token)
{
  assert(token != NULL);

  if (token->type == FUNCTION)
    print_function(token->data);
  else if (is_operator(token->type))
    print_operator(token->data);
  else
    printf("%s", (char*)token->data);
}


/**
 * @brief Deletes a token
 *
 * @param token The token to delete
 */
static void delete_token(Token token)
{
  assert(token != NULL);

  free(token->data);
  free(token);
}


/**
 * @brief Creates a copy of a given token
 *
 * @param token The token to clone
 * @return The address of the new token
 * @see Operator::is_operator, Operator::clone_operator, Function::clone_function
 */
Token clone_token(Token token)
{
  Token copy = malloc(sizeof(*copy));
  assert(copy != NULL);

  copy->type    = token->type;
  copy->print   = &print_token;
  copy->destroy = &delete_token;

  if (is_operator(token->type)) {
    copy->data = clone_operator(token->data);
  } else if (token->type == FUNCTION) {
    copy->data = clone_function(token->data);
  } else {
    copy->data = malloc(strlen(token->data)+1);
    assert(copy->data != NULL);
    strcpy(copy->data, token->data);
  }

  return copy;
}


/**
 * @brief Creates a new token
 *
 * @param type The token type
 * @param value The value to store
 *
 * @return The address of the created token
 * @see Operator::is_operator, Operator::create_operator, Function::create_function
 */
Token create_token(TokenType type, const char *value)
{
  Token token = malloc(sizeof(*token));
  assert(token != NULL);

  token->type    = type;
  token->print   = &print_token;
  token->destroy = &delete_token;

  if (is_operator(type)) {
    token->data = create_operator(type, value);
  } else if (type == FUNCTION) {
    token->data = create_function(value);
  } else {
    token->data = malloc(strlen(value)+1);
    assert(token->data != NULL);
    strcpy(token->data, value);
  }

  return token;
}


/**
 * @brief Gets the type of a given token
 *
 * @param token The token
 * @return The token type
 */
TokenType get_type(Token token)
{
  assert(token != NULL);
  return token->type;
}