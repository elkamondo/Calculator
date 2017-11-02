#ifndef NODE_H
#define NODE_H

/**
 * @brief Represents the type of the node stored in the stack node
 */
typedef enum DataType { TOKEN, ASTNODE } DataType;

typedef void *Element;

/**
 * @brief The stack node which holds the token/ASTNode
 */
typedef struct stack_elt_t *StackNode;
typedef struct stack_elt_t
{
  DataType dtype;
  Element val;
  StackNode next;

  void (*print_elt)(StackNode);
} stack_elt_t;

#endif