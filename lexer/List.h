#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "Token.h"

/**
 * @brief The linked list node which holds the token
 */
typedef struct _token_node
{
  Token data;
  struct _token_node *next;
} token_node, *TokenNode;

/**
 * @brief The linked list where to store the tokens
 */
typedef struct list_t *List;
typedef struct list_t
{
  TokenNode head;
  TokenNode tail;

  void (*add)(List, Token);
  bool (*is_empty)(List);
  void (*destroy)(List);
} list_t;

/**
 * @brief Creates a new linked list
 */
List create_token_list(void);

/**
 * @brief Tokenize a mathematic expression
 */
List tokenize_expression(const char*);

#endif