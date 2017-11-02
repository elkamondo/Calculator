#include <stdio.h>
#include <stdlib.h>

#include "lexer/List.h"
#include "parser/AST.h"
#include "parser/Parser.h"

int main(void)
{
  char expression[256];
  printf("\nEnter your mathematical expression:\n  -> ");
  if (!fgets(expression, sizeof expression, stdin))
  {
    fprintf(stderr, "Can't read your input!");
    exit(EXIT_FAILURE);
  }

  List list = tokenize_expression(expression);
  ASTNode root = parse_expression(list);

  root = eval_tree(root);

  root->destroy(root);
  list->destroy(list);
}