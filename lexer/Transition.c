#include "../CommonHeaders.h"
#include "Transition.h"
#include "Token.h"


/**
 * @brief Creates a transition table
 * @details Allocates space for the transition table of the deterministic finite
 *          automata, which to be used to tokenize the mathematical expression.
 *
 * @return The transition table
 */
TransitionTable create_transition_table(void)
{
  TransitionTable transition = calloc((NUM_STATES+1), sizeof(*transition));
  assert(transition != NULL);

  for (size_t i = 0; i <= NUM_STATES; ++i) {
    transition[i] = calloc(MAX_CHARS_LENGTH, sizeof(**transition));
    assert(transition[i] != NULL);
  }

  return transition;
}


/**
 * @brief Fill the transition table
 * @details The deterministic finite automata recognizes:
 *            . function names (identifiers):  [a-zA-Z][a-zA-Z0-9]*
 *            . floating-point numbers - like 3.12, 5, 6.23e12 -:
 *                ([0-9]+(\.[0-9]*)? | (\.[0-9]+))(([eE][+-]?[0-9])?)
 *            . arithmetic operators: '^', '*', '+', '-', '%', '/'
 *            . parenthesis : '(', ')'
 *            . the comma function argument separator: ','
 *
 * @param transition The transition table to fill
 */
void generate_transition_table(TransitionTable transition)
{
  transition[0]['('] = 1;
  transition[0][')'] = 2;
  transition[0]['^'] = 3;
  transition[0]['*'] = 4;
  transition[0]['/'] = 5;
  transition[0]['+'] = 6;
  transition[0]['-'] = 7;
  transition[0]['%'] = 8;
  transition[0][','] = 9;
  transition[0]['.'] = 19;

  for (size_t i = 0; i < MAX_CHARS_LENGTH; ++i) transition[10][i] = 11;
  for (size_t i = 0; i < MAX_CHARS_LENGTH; ++i) transition[12][i] = 13;
  for (size_t i = 0; i < MAX_CHARS_LENGTH; ++i) transition[14][i] = 18;
  for (size_t i = 0; i < MAX_CHARS_LENGTH; ++i) transition[17][i] = 18;

  for (size_t i = 'A'; i <= 'Z'; ++i) transition[0][i]  = 10;
  for (size_t i = 'a'; i <= 'z'; ++i) transition[0][i]  = 10;
  for (size_t i = 'A'; i <= 'Z'; ++i) transition[10][i] = 10;
  for (size_t i = 'a'; i <= 'z'; ++i) transition[10][i] = 10;

  for (size_t i = '0'; i <= '9'; ++i) transition[0][i]  = 12;
  for (size_t i = '0'; i <= '9'; ++i) transition[12][i] = 12;
  for (size_t i = '0'; i <= '9'; ++i) transition[14][i] = 14;
  for (size_t i = '0'; i <= '9'; ++i) transition[15][i] = 17;
  for (size_t i = '0'; i <= '9'; ++i) transition[16][i] = 17;
  for (size_t i = '0'; i <= '9'; ++i) transition[17][i] = 17;
  for (size_t i = '0'; i <= '9'; ++i) transition[19][i] = 14;

  transition[12]['.'] = 14;
  transition[12]['e'] = 15;
  transition[12]['E'] = 15;
  transition[14]['e'] = 15;
  transition[14]['E'] = 15;
  transition[15]['+'] = 16;
  transition[15]['-'] = 16;
}


/**
 * @brief Deallocates space allocated by a transition table
 *
 * @param transition The transition table to delete
 */
void delete_transition_table(TransitionTable transition)
{
  for (size_t i = 0; i <= NUM_STATES; ++i)
    free(transition[i]);
  free(transition);
}


/**
 * @brief Creates a table holds the final states of the deterministic finite automata
 *
 * @return The address of the table
 */
int *create_final_table(void)
{
  int *finals_table = calloc((NUM_STATES+1), sizeof(*finals_table));
  assert(finals_table != NULL);

  finals_table[1]  = LPARENTHESIS;
  finals_table[2]  = RPARENTHESIS;
  finals_table[3]  = EXPONENT;
  finals_table[4]  = MULTIPLY;
  finals_table[5]  = DIVIDE;
  finals_table[6]  = PLUS;
  finals_table[7]  = MINUS;
  finals_table[8]  = MODULO;
  finals_table[9]  = FARGSEPARATOR;
  finals_table[11] = -FUNCTION;
  finals_table[13] = -LITERAL;
  finals_table[18] = -LITERAL;

  return finals_table;
}


/**
 * @brief Deallocates space allocated by the table that holds the final states
 *        of the deterministic finite automata (DFA)
 *
 * @param final_table The table to delete
 */
void delete_final_table(int *final_table)
{
  free(final_table);
}