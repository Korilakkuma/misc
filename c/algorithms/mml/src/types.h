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

typedef struct Tree {
  Token *token;
  struct Tree *left;
  struct Tree *right;
} Tree;

extern void tokenize(char *in, Token tokens[]);
extern void tokenize_print(const Token tokens[], const unsigned long number_of_tokens);

extern void tree_construct(Token tokens[], Tree trees[]);
extern void tree_destruct(const Tree trees[]);
extern void tree_print(const Tree trees[]);
