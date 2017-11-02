#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../CommonHeaders.h"
#include "List.h"
#include "Transition.h"
#include "Operator.h"
#include "Token.h"


/**
 * @brief Checks if a given linked list is empty
 *
 * @param list The linked list
 * @return true if the linked list is empty, false otherwise
 */
static bool empty_list(List list)
{
  return list->head == NULL && list->tail == NULL;
}


/**
 * @brief Adds a token into a linked list
 * @details Takes a given token, and allocates space for it.
 *          Then insert it into the linked list
 *
 * @param list The linked list where to insert the token
 * @param token The token to insert
 * @see Token
 */
static void add_token(List list, Token token)
{
  TokenNode tnode = malloc(sizeof(*tnode));
  assert(tnode != NULL);

  tnode->data = token;
  tnode->next = NULL;

  if (list->is_empty(list)) {
    list->head = tnode;
    list->tail = tnode;
  } else {
    list->tail->next = tnode;
    list->tail = tnode;
  }
}


/**
 * @brief Deletes a given singly linked list
 * @details If there is any element in the list, then deletes it.
 *
 * @param list The linked list to delete
 * @see Token::destroy
 */
static void delete_token_list(List list)
{
  TokenNode current = list->head;

  while (current) {
    TokenNode temp = current;
    current = current->next;
    (temp->data)->destroy(temp->data);
    free(temp);
  }

  free(list);
}


/**
 * @brief Creates a new linked list
 *
 * @return The address of the created linked list
 */
List create_token_list(void)
{
  List list = malloc(sizeof(*list));
  assert(list != NULL);

  list->head = NULL;
  list->tail = NULL;

  list->add      = &add_token;
  list->is_empty = &empty_list;
  list->destroy  = &delete_token_list;

  return list;
}


/**
 * @brief Tokenize a mathematic expression
 * @details Takes a string represents a mathematic expression, and generates
 *          a list of tokens using a deterministic finite automata (DFA).
 *
 * @param expression String represents the mathematic expression
 * @return The address of the list which holds the tokens
 * @see Transition::create_transition_table, Transition::generate_transition_table,
 *      Transition::create_final_table, Transition::delete_final_table,
 *      Transition::delete_transition_table, Token::create_token,
 *      Operator::is_operator
 */
List tokenize_expression(const char *expression)
{
  FILE *in = fopen("expression.in", "w+");
  assert(in != NULL);
  fprintf(in, "%s", expression);
  rewind(in);

  TransitionTable transition = create_transition_table();
  generate_transition_table(transition);

  int *final = create_final_table();

  List list = create_token_list();

  char *lexeme = calloc(strlen(expression) + 1, sizeof(*lexeme));
  assert(lexeme != NULL);

  int c = 0;
  int prev_token = -1;
  while ((c = fgetc(in)) != EOF)
  {
    while (isspace(c) && (c = fgetc(in)) != EOF);

    size_t state = 0, i = 0;
    do
    {
      lexeme[i++] = (char)c;
      if (c != EOF) state = transition[state][c];
    } while (!final[state] && (c = fgetc(in)) != EOF);

    if (final[state] < 0)
    {
      ungetc(c, in);
      lexeme[i-1] = '\0';
    } else {
      lexeme[i] = '\0';
    }

    int current_token = abs(final[state]);

    if (current_token == MINUS ) {
      if (prev_token == -1
        || is_operator(prev_token)
        || prev_token == LPARENTHESIS
        || prev_token == FARGSEPARATOR) {
        list->add(list, create_token(UMINUS, lexeme));
      } else {
        list->add(list, create_token(BMINUS, lexeme));
      }
    } else {
        list->add(list, create_token(current_token, lexeme));
    }

    prev_token = current_token;
  }

  fclose(in);
  remove("expression.in");
  free(lexeme);
  delete_final_table(final);
  delete_transition_table(transition);

  return list;
}