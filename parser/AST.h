#ifndef AST_H
#define AST_H

#include "../lexer/Token.h"

/**
 * @brief The tree node which holds the token and its children
 */
typedef struct ast_t *ASTNode;
typedef struct ast_t
{
  Token token;
  ASTNode left;
  ASTNode right;

  void (*print)(ASTNode);
  void (*destroy)(ASTNode);
} ast_t;

/**
 * @brief Creates a new AST node
 */
ASTNode create_ast_node(Token, ASTNode, ASTNode);

/**
 * @brief Evaluates the parse tree
 */
ASTNode eval_tree(ASTNode);

#endif