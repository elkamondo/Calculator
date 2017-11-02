#include <math.h>
#include <float.h>

#include "../CommonHeaders.h"
#include "AST.h"
#include "../lexer/Operator.h"
#include "../lexer/Function.h"
#include "../lexer/Token.h"


/**
 * @brief Prints the parse tree
 * @details If the root type is a function or an operator, then print operands
 *          between parenthesis to make the expression more readable and
 *          avoid confusion.
 *          example: 3 * 5 - 2   -> ((3 * 5) - 2)
 *                   3 * (5 - 2) -> (3 * (5 - 2))
 *
 * @param root The root of the tree
 * @see Function::print_function, Function::get_function_type, Token::print
 */
static void print_ast(ASTNode root)
{
  if (root) {
    if (root->token->type == FUNCTION) {
      print_function(root->token->data);
      printf("(");
      print_ast(root->left);

      if (get_function_type(root->token->data) == BINARY)
        printf(",");

      print_ast(root->right);
      printf(")");
    } else {
      if (root->token->type != LITERAL) printf("(");
      print_ast(root->left);
      (root->token)->print(root->token);
      print_ast(root->right);
      if (root->token->type != LITERAL) printf(")");
    }
  }
}


/**
 * @brief Deletes a parse tree
 *
 * @param root The root of the tree
 * @see Token::destroy
 */
static void destroy_ast(ASTNode root)
{
  if (root) {
    destroy_ast(root->left);
    destroy_ast(root->right);
    (root->token)->destroy(root->token);
    free(root);
  }
}


/**
 * @brief Creates a new AST node
 *
 * @param token The token
 * @param left The left child
 * @param right The right child
 * @return The address of the created node
 */
ASTNode create_ast_node(Token token, ASTNode left, ASTNode right)
{
  ASTNode node = malloc(sizeof(*node));
  assert(node != NULL);

  node->token = token;
  node->left  = left;
  node->right = right;

  node->print   = &print_ast;
  node->destroy = &destroy_ast;

  return node;
}


/**
 * @brief Gets the address of the first operator the evaluate and
 *        the address of its parent
 * @details example: 3 * (5 - 2)
 *          Tree:  *      -> then the first operator to evaluate is '-'
 *               /  \        and its parent is '*'
 *              3    -
 *                  / \
 *                 5  2
 *
 * @param root The root of the tree
 * @param first_op Where to store the address of the first operator to evaluate
 * @param p Where to store the address of its parent
 */
static void get_first_operator(ASTNode root, ASTNode *first_op, ASTNode *p)
{
  assert(root != NULL);

  ASTNode loperator = root, parent = NULL;
  while (loperator) {
    if (loperator->right && loperator->right->right) {
      parent = loperator;
      loperator = loperator->right;
    } else if (loperator->left && loperator->left->right) {
      parent = loperator;
      loperator = loperator->left;
    } else {
      break;
    }
  }

  *first_op = loperator;
  *p = parent;
}


/**
 * @brief Evaluates the parse tree step by step
 * @details If there is any elements in the tree, then evaluate
 *          the first operator and store the result in a new node,
 *          which to be inserted instead of the first operator evaluated.
 *
 *          example: 3 * (5 - 2)
 *          Tree:  *      ->      *
 *               /  \            / \
 *              3    -          3  3
 *                  / \
 *                 5  2
 *
 * @param root The root of the tree
 * @return The root address of the updated tree
 * @see Function::eval_function, Operator::eval_operator, Token::create_token,
 *      Token::destroy
 */
static ASTNode evaluate_step_by_step(ASTNode root)
{
  ASTNode parent = NULL, first_op = NULL;
  get_first_operator(root, &first_op, &parent);

  long double lc = 0.0;
  if (first_op->left) lc = strtod(first_op->left->token->data, NULL);

  long double rc = strtod(first_op->right->token->data, NULL);

  long double result = 0.0;
  if (first_op->token->type == FUNCTION)
    result = eval_function(first_op->token->data, lc, rc);
  else
    result = eval_operator(first_op->token->type, lc, rc);

  char str[128];

  // If there is no fractional part, store the whole part of the number
  if ((result - floorl(result)) < DBL_EPSILON)
    sprintf(str, "%.*Lf", 0, result);
  else
    sprintf(str, "%.*Lf", DBL_DIG, result);

  ASTNode node = create_ast_node(create_token(LITERAL, str), NULL, NULL);

  if (!parent) root = node;
  else if (parent->left && parent->left->token == first_op->token)
    parent->left = node;
  else
    parent->right = node;

  (first_op)->destroy(first_op);

  return root;
}


/**
 * @brief Evaluates the parse tree
 *
 * @param root The root of the tree
 * @return The root address of the evaluated tree
 * @see Token::print
 */
ASTNode eval_tree(ASTNode root)
{
  assert(root != NULL);

  while (root->left || root->right)
  {
    printf("\n\t= ");
    root = evaluate_step_by_step(root);
    root->print(root);
    printf("\n");
  }
  
  return root;
}