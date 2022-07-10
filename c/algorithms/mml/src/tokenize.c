#include "includes.h"
#include "types.h"

static char *get_token(char *in, Token *const token);
static TokenTypes get_token_type(const char c);

static TokenMap token_map[] = {
  {'T', TEMPO},
  {'O', OCTAVE},
  {'C', NOTE},
  {'D', NOTE},
  {'E', NOTE},
  {'F', NOTE},
  {'G', NOTE},
  {'A', NOTE},
  {'B', NOTE},
  {'+', NOTE},
  {'-', NOTE},
  {'R', REST},
  {'0', NUMBER},
  {'1', NUMBER},
  {'2', NUMBER},
  {'3', NUMBER},
  {'4', NUMBER},
  {'5', NUMBER},
  {'6', NUMBER},
  {'7', NUMBER},
  {'8', NUMBER},
  {'9', NUMBER},
  {'.', NUMBER},
  {'\0', EOM}
};

void tokenize(char *in, Token tokens[]) {
  while (1) {
    Token token;

    // Temporary values (Overridden by `get_token` function)
    token.type     = UNKNOWN_TOKEN_ERROR;
    token.token[0] = '\0';
    token.value    = -1;

    in = get_token(in, &token);

    switch (token.type) {
      case BUFFER_OVERFLOW_ERROR:
        exit(EXIT_FAILURE);
      case UNKNOWN_TOKEN_ERROR:
        exit(EXIT_FAILURE);
      default:
        *tokens++ = token;
        break;
    }

    if (token.type == EOM) {
      break;
    }
  }
}

void tokenize_print(const Token tokens[], const unsigned long number_of_tokens) {
  for (int i = 0; i < number_of_tokens; i++) {
    Token token = tokens[i];

    char *type;

    switch (token.type) {
      case BUFFER_OVERFLOW_ERROR:
        fprintf(stderr, "Error: Buffer Overflow\n");
        exit(EXIT_FAILURE);
      case UNKNOWN_TOKEN_ERROR:
        fprintf(stderr, "Error: %s is Unknown Token\n", token.token);
        exit(EXIT_FAILURE);
      default:
        switch (token.type) {
          case TEMPO:
            type = "TEMPO";
            break;
          case OCTAVE:
            type = "OCTAVE";
            break;
          case NOTE:
            type = "NOTE";
            break;
          case REST:
            type = "REST";
            break;
          case NUMBER:
            type = "NUMBER";
            break;
          case BUFFER_OVERFLOW_ERROR:
          case UNKNOWN_TOKEN_ERROR:
            exit(EXIT_FAILURE);
          case EOM:
          default :
            type = "EOM";
            break;
        }

        fprintf(stdout, "%-6s %4s (value=%3d)\n", type, token.token, token.value);
        break;
    }

    if (token.type == EOM) {
      break;
    }
  }
}

static char *get_token(char *in, Token *const token) {
  while (*in == ' ') {
    ++in;
  }

  TokenTypes t = get_token_type(*in);

  if (t == EOM) {
    token->type     = EOM;
    token->token[0] = *in;
    return in;
  }

  switch (t) {
    case TEMPO:
      token->type     = t;
      token->token[0] = *in;
      token->token[1] = '\0';
      break;
    case OCTAVE:
      token->type     = t;
      token->token[0] = *in;
      token->token[1] = '\0';
      break;
    case NOTE:
      token->type     = t;
      token->token[0] = *in;
      token->token[1] = '\0';

      // Look-ahead
      ++in;

      if ((*in == '+') || (*in == '-')) {
        token->token[1] = *in;
        token->token[2] = '\0';
      } else {
        // Look-behind
        --in;
      }

      break;
    case REST:
      token->type     = t;
      token->token[0] = *in;
      token->token[1] = '\0';
      break;
    case NUMBER:
      token->token[0] = *in;
      token->token[1] = '\0';

      // Look-ahead
      while ((*(++in) != '\0') && (get_token_type(*in) == NUMBER)) {
        size_t len = strlen(token->token);

        if (len < (TOKEN_LENGTH - 1)) {
          token->token[len + 0] = *in;
          token->token[len + 1] = '\0';
        } else {
          token->type     = BUFFER_OVERFLOW_ERROR;
          token->token[0] = *in;
          token->token[1] = '\0';

          return in;
        }
      }

      token->type  = NUMBER;
      token->value = (int)strtol(token->token, NULL, 10);

      // Look-behind
      --in;

      break;
    default:
      token->type     = UNKNOWN_TOKEN_ERROR;
      token->token[0] = *in;
      token->token[1] = '\0';
      break;
  }

  return ++in;
}


static TokenTypes get_token_type(const char c) {
  if (c == '\0') {
    return EOM;
  }

  for (int i = 0; token_map[i].token != '\0'; i++) {
    if (c == token_map[i].token) {
      return token_map[i].type;
    }
  }

  return UNKNOWN_TOKEN_ERROR;
}
