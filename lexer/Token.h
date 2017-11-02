#ifndef TOKEN_H
#define TOKEN_H


/**
 * @brief Represents the type of the token's ID
 */
typedef enum token_type
{
  LITERAL  = 1,
  FUNCTION,
  FARGSEPARATOR,
  LPARENTHESIS,
  RPARENTHESIS,
  EXPONENT,
  MULTIPLY,
  DIVIDE,
  PLUS,
  MINUS,
  UMINUS,
  BMINUS,
  MODULO
} TokenType;

/**
 * @brief The node which holds the token information
 */
typedef struct token_t *Token;
typedef struct token_t {
  TokenType type;
  void *data;

  void (*print)(Token);
  void (*destroy)(Token);
} token_t;

/**
 * @brief Creates a new token
 */
Token create_token(TokenType, const char*);

/**
 * @brief Creates a copy of a given token
 */
Token clone_token(Token);

/**
 * @brief Gets the type of a given token
 */
TokenType get_type(Token);

#endif