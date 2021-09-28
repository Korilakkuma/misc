#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_LENGTH 64

typedef enum {
  TEMPO,
  OCTAVE,
  NOTE,
  REST,
  NUMBER,
  BUFFER_OVERFLOW_ERROR,
  UNKNOWN_TOKEN_ERROR,
  EOM  // End Of MML
} TokenTypes;

typedef struct {
  char token;
  TokenTypes type;
} TokenMap;

typedef struct {
  TokenTypes type;
  char token[TOKEN_LENGTH];
  int value;
} Token;

extern char *tokenize(char *in, Token *token);
extern void tokenize_print(char *in, Token *tokens);
