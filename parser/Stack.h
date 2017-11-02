#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "StackNode.h"

/**
 * @brief The stack where to store the tokens
 */
typedef struct stack_t* Stack;
typedef struct stack_t
{
  StackNode head;

  void (*push)(Stack, Element, DataType);
  Element (*top)(Stack);
  void (*pop)(Stack);
  bool (*is_empty)(Stack);
} stack_t;

/**
 * @brief Creates a new stack
 */
Stack create_stack(void);

/**
 * @brief Deletes a stack
 */
void delete_stack(Stack);

/**
 * @brief Adds operands to operators/functions
 */
void add_operand_to_operator(Stack, Stack);

#endif