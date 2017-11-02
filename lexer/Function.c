#include <strings.h>
#include <math.h>

#include "../CommonHeaders.h"
#include "Function.h"


/**
 * @brief Creates a function type
 * @details If the given name is not a valid mathematical function,
 *          the programs fails with an error. If it's valid then
 *          creates a new function type.
 *
 * @param name The name of the function
 * @return The address of the created function type
 */
Function create_function(const char *name)
{
  Function function = malloc(sizeof(*function));
  assert(function != NULL);

  FunctionID funcName = get_function_id(name);
  if (funcName == NONE)
  {
    fprintf(stderr, "'%s' is not a function\n", name);
    exit(EXIT_FAILURE);
  }

  function->id = funcName;
  function->type = get_function_type(function);

  return function;
}


/**
 * @brief Returns an ID represents a mathematical function
 *
 * @param name The name of the function
 * @return The ID of the function
 */
FunctionID get_function_id(const char *name)
{
  if (!strcasecmp(name, "sin"))  return SIN;
  if (!strcasecmp(name, "cos"))  return COS;
  if (!strcasecmp(name, "tan"))  return TAN;
  if (!strcasecmp(name, "sqrt")) return SQRT;
  if (!strcasecmp(name, "abs"))  return ABS;
  if (!strcasecmp(name, "ln"))   return LN;
  if (!strcasecmp(name, "max"))  return MAX;
  if (!strcasecmp(name, "min"))  return MIN;

  return NONE;
}


/**
 * @brief Returns a copy of a given function type
 *
 * @param func The function to clone
 * @return The copy of the function
 */
Function clone_function(Function func)
{
  Function copy = malloc(sizeof(*copy));
  assert(copy != NULL);

  copy->id   = func->id;
  copy->type = func->type;

  return copy;
}


/**
 * @brief Returns the type of a given function
 * @details The Unary function??
 *
 * @param func The function
 * @return The type of the function
 */
FunctionType get_function_type(Function func)
{
  if (func->id >= TOTAL_UNARY_FUNCTIONS)
    return BINARY;

  return UNARY;
}


/**
 * @brief Prints the name of a given function type
 *
 * @param function The function to print
 */
void print_function(Function function)
{
  const char *funcs[] = { "sin", "cos", "tan", "sqrt", "abs", "ln", "max", "min" };
  printf("%s", funcs[function->id]);
}


/**
 * @brief Evaluates a function
 *
 * @param func The function to evaluate
 * @param lc The first operand
 * @param rc The second operand
 * @return The result of the function evaluation
 */
long double eval_function(Function func, long double lc, long double rc)
{
  long double (*bfunc[])(long double, long double) = { fmaxl, fminl };
  long double (*ufunc[])(long double) = { sinl, cosl, tanl, sqrtl, fabsl, logl };

  long double result = 0.0;
  if (get_function_type(func) == UNARY)
    result = ufunc[func->id](rc);
  else
    result = bfunc[func->id - TOTAL_UNARY_FUNCTIONS](lc, rc);

  return result;
}