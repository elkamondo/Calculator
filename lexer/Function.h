#ifndef FUNCTION_H
#define FUNCTION_H

#define TOTAL_FUNCTIONS 8
#define TOTAL_UNARY_FUNCTIONS 6

/**
 * @brief Represents the type of the function ID
 */
typedef enum function_id { SIN, COS, TAN, SQRT, ABS, LN, MAX, MIN, NONE } FunctionID;

/**
 * @brief Represents the type of the function type
 */
typedef enum function_type { UNARY = 1, BINARY } FunctionType;

/**
 * @brief Represents the function type which holds the function information
 */
typedef struct func_t
{
  FunctionID id;
  FunctionType type;
} func_t, *Function;

/**
 * @brief Creates a function type
 */
Function create_function(const char*);

/**
 * @brief Returns an ID represents a mathematical function
 */
FunctionID get_function_id(const char*);

/**
 * @brief Returns the type of a given function
 */
FunctionType get_function_type(Function);

/**
 * @brief Prints the name of a given function type
 */
void print_function(Function);

/**
 * @brief Returns a copy of a given function type
 */
Function clone_function(Function);

/**
 * @brief Evaluates a function
 */
long double eval_function(Function, long double, long double);

#endif