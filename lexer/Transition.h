#ifndef TRANSITION_H
#define TRANSITION_H

#define NUM_STATES 19
#define MAX_CHARS_LENGTH 128

/**
 * @brief Represents the type of the transition table
 */
typedef unsigned int **TransitionTable;

/**
 * @brief Creates a transition table
 */
TransitionTable create_transition_table(void);

/**
 * @brief Fill the transition table
 */
void generate_transition_table(TransitionTable);

/**
 * @brief Deallocates space allocated by a transition table
 */
void delete_transition_table(TransitionTable);

/**
 * @brief Creates a table holds the final states of the deterministic finite automata
 */
int *create_final_table(void);

/**
 * @brief Deallocates space allocated by the table that holds the final states
 *        of the deterministic finite automata (DFA)
 */
void delete_final_table(int*);

#endif