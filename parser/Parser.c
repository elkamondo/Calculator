#include <stdio.h>
#include <stdlib.h>

#include "../lexer/Token.h"
#include "../lexer/List.h"
#include "../lexer/Operator.h"

#include "Stack.h"
#include "AST.h"


/**
 * @brief Creates a parse tree from a list of tokens
 * @details Implementing the shunting yard algorithm.
 *
 *          While there are tokens to be read:
 *            1. Read the token
 *            2. If it's a literal token, push it on the output stack.
 *            3. If it's a function token, push it on the operators stack.
 *            4. If it's a function argument separator, pop operators off the
 *               the stack, add their operands to them and then push them onto
 *               the output stack until a left parenthesis is encountered.
 *            5. If it's an operator: if the operator is associative and has
 *               lesser precedence than the operator below it on the operators
 *               stack, pop the lower operator off the operators stack, add
 *               their operands to it and push it to the output stack.
 *            6. If it's a left parenthesis, push it onto the operators stack
 *            7. If it's a right parenthesis, pop operators off the operators
 *               stack, add their operands to them and then push them onto the
 *               output stack until a left parenthesis is encountered.
 *
 *               If the top of the operators stack is a function token, add
 *               the operands to it, and then push it onto the output stack.
 *
 *            When there are no more tokens to read, pop operators off add
 *            their operands to them and then push them onto the output stack.
 *
 * @param list The list of tokens
 * @return The root of the parse tree
 * @see Stack, Stack::create_stack, Token, Token::get_type, Token::clone_token,
 *      ASTNode::create_ast_node, Stack::add_operand_to_operator, List, Operator
 */
ASTNode parse_expression(List list)
{
  if (!list) return NULL;

  Stack output = create_stack();
  Stack operators = create_stack();

  TokenNode ptr = list->head;
  while (ptr) {
    if (get_type(ptr->data) == LITERAL) {

      ASTNode node = create_ast_node(clone_token(ptr->data), NULL, NULL);
      output->push(output, node, ASTNODE);

    } else if (get_type(ptr->data) == FUNCTION) {

      operators->push(operators, clone_token(ptr->data), TOKEN);

    } else if (get_type(ptr->data) == FARGSEPARATOR) {

      while (operators->top(operators)
        && get_type(operators->top(operators)) != LPARENTHESIS) {
        add_operand_to_operator(output, operators);
      }

    } else if (is_operator(get_type(ptr->data))) {

      while (!(operators->is_empty(operators))
          && is_operator(get_type(operators->top(operators)))) {

        Token temp = operators->top(operators);
        AssocType asso = ((Operator)ptr->data->data)->associativity;
        unsigned int prec1 = ((Operator)ptr->data->data)->precedence;
        unsigned int prec2 = ((Operator)temp->data)->precedence;

        if ((asso == LEFT && prec1 <= prec2) || (asso == RIGHT && prec1 < prec2)) {
          add_operand_to_operator(output, operators);
        } else {
          break;
        }
      }

      operators->push(operators, clone_token(ptr->data), TOKEN);

    } else if (get_type(ptr->data) == LPARENTHESIS) {

      operators->push(operators, clone_token(ptr->data), TOKEN);

    } else if (get_type(ptr->data) == RPARENTHESIS) {

      while (operators->top(operators)
          && get_type(operators->top(operators)) != LPARENTHESIS) {
        add_operand_to_operator(output, operators);
      }

      operators->pop(operators);

      if (operators->top(operators)
       && get_type(operators->top(operators)) == FUNCTION) {
        add_operand_to_operator(output, operators);
      }
    }

    ptr = ptr->next;
  }

  while (!(operators->is_empty(operators))) {
    if (get_type(operators->top(operators)) == LPARENTHESIS) {
      fprintf(stderr, "Error: Unmatched parenthesis\n");
      exit(EXIT_FAILURE);
    }

    add_operand_to_operator(output, operators);
  }

  ASTNode temp = output->top(output);
  output->pop(output);

  if (!(output->is_empty(output))) {
    fprintf(stderr, "Error: Invalid expression\n");
    exit(EXIT_FAILURE);
  }

  delete_stack(operators);
  delete_stack(output);

  return temp;
}