#include <stdbool.h>

#include "../CommonHeaders.h"
#include "Stack.h"
#include "StackNode.h"
#include "AST.h"
#include "../lexer/Token.h"
#include "../lexer/Operator.h"
#include "../lexer/Function.h"


/**
 * @brief Checks if a stack is empty
 *
 * @param s The stack to check
 * @return true if the stack is empty, false otherwise
 */
static bool is_empty_stack(Stack s)
{
  return s->head == NULL;
}


/**
 * @brief Push a new element onto the stack
 *
 * @param s The stack
 * @param elt The element to store
 * @param dtype The type of the new element (ASTNode or Token)
 */
static void push_stack(Stack s, Element elt, DataType dtype)
{
  StackNode node = malloc(sizeof(*node));
  assert(node != NULL);

  node->dtype = dtype;
  node->val   = elt;
  node->next  = s->head;

  s->head = node;
}


/**
 * @brief Returns the element at the top of the stack
 *
 * @param s The stack
 * @return The element at the top of the stack
 */
static Element top_stack(Stack s)
{
  if (s->is_empty(s)) return NULL;

  return s->head->val;
}


/**
 * @brief Pop the top element off the stack
 *
 * @param s The stack to pop
 * @see Token::destroy
 */
static void pop_stack(Stack s)
{
  if (!(s->is_empty(s))) {
    StackNode temp = s->head;
    s->head = temp->next;

    if (temp->dtype == TOKEN)
      ((Token)temp->val)->destroy((Token)temp->val);

    free(temp);
  }
}


/**
 * @brief Creates a new stack
 *
 * @return The address of the stack newly created
 */
Stack create_stack(void)
{
  Stack stack = malloc(sizeof(*stack));
  stack->head = NULL;

  stack->push = &push_stack;
  stack->top  = &top_stack;
  stack->pop  = &pop_stack;
  stack->is_empty = &is_empty_stack;

  return stack;
}


/**
 * @brief Deletes the content of a stack node
 *
 * @param snode The stack node which to delete its content
 * @see Token::destroy, ASTNode::destroy
 */
static void delete_elt(StackNode snode)
{
  if (snode) {
    if (snode->dtype == TOKEN)
      ((Token)snode->val)->destroy(snode->val);
    else
      ((ASTNode)snode->val)->destroy(snode->val);

    free(snode);
  }
}


/**
 * @brief Deletes a stack
 * @details If there are any nodes in the stack, deletes them
 *          with their contents
 *
 * @param stack The stack to delete
 */
void delete_stack(Stack stack)
{
  while (!(stack->is_empty(stack))) {
    StackNode temp = stack->head;
    stack->pop(stack);
    delete_elt(temp);
  }

  free(stack);
}


/**
 * @brief Adds operands to operators/functions
 * @details If it's a binary operator/function, create a tree which the
 *          the operator/function is root, and has two operands which are
 *          the childrens. If an operand is missing, stop the program with
 *          an error: 'Invalid expression'.
 *
 *          If it's a unary operator/function, create a tree which the
 *          the operator/function is root, and has one operand which is the
 *          right child.
 *
 * @param output The output stack
 * @param operators The operators stack
 * @see Token, Token::clone_token, Function::get_function_type,
 *      ASTNode::create_ast_node
 */
void add_operand_to_operator(Stack output, Stack operators)
{
  Token root = clone_token(operators->top(operators));
  operators->pop(operators);


  ASTNode right_child = (ASTNode)output->top(output);
  output->pop(output);

  if ((root->type == FUNCTION && get_function_type(root->data) == UNARY)
   || (root->type == UMINUS)) {
    output->push(output, create_ast_node(root, NULL, right_child), ASTNODE);
  } else {

    ASTNode left_child = (ASTNode)output->top(output);
    if (!left_child) {
      fprintf(stderr, "Invalid expression\n");
      exit(EXIT_FAILURE);
    }

    output->pop(output);

    output->push(output, create_ast_node(root, left_child, right_child), ASTNODE);
  }
}