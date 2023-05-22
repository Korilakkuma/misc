#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SIZE_OF_STACK  20
#define SIZE_OF_BUFFER 80

typedef enum {
  LEFT_PAREN,
  RIGHT_PAREN,
  PLUS,
  MINUS,
  MULTI,
  DIVIDE,
  ASSIGN,
  INT,
  VARIABLE,
  PRINT,
  EOF_TOKEN,
  OTHERS,
} TokenType;

typedef struct {
  TokenType type;
  int value;
} Token;

void input(void);
void statement(void);
Token next_token(void);

static void expression(void);
static void term(void);
static void factor(void);

static int next_char(void);

static void operate(TokenType op);
static void push(int n);
static int pop(void);

static bool check_token(TokenType type);

static int stack[SIZE_OF_STACK];
static int stack_pointer = 0;

static Token token;

static char buf[SIZE_OF_BUFFER];
static char *pbuf;

static int ch;
static int var[26];
static bool has_error;

int main(int argc, char **argv) {
  while (true) {
    input();

    token = next_token();

    if (token.type == EOF_TOKEN) {
      exit(EXIT_FAILURE);
    }

    statement();

    if (has_error) {
      fputs("Error occurred\n", stderr);
    }
  }

  return 0;
}

void input(void) {
  has_error     = false;
  stack_pointer = 0;

  buf[0] = '\0';

  fputs("Type statement > ", stdout);
  fgets(buf, SIZE_OF_BUFFER, stdin);

  pbuf = buf;

  ch = next_char();
}

void statement(void) {
  switch (token.type) {
    case VARIABLE: {
      int v = token.value;

      token = next_token();

      has_error = check_token(ASSIGN);

      if (has_error) {
        break;
      }

      token = next_token();

      expression();

      var[v] = pop();
      break;
    }

    case PRINT: {
      token = next_token();

      expression();

      has_error = check_token(EOF_TOKEN);

      if (has_error) {
        break;
      }

      printf("  %d\n", pop());
      return;
    }

    default: {
      has_error = true;
      break;
    }
  }

  has_error = check_token(EOF_TOKEN);
}

static void expression(void) {
  TokenType op;

  term();

  while ((token.type == PLUS) || (token.type == MINUS)) {
    op    = token.type;
    token = next_token();

    term();
    operate(op);
  }
}

static void term(void) {
  TokenType op;

  factor();

  while ((token.type == MULTI) || (token.type == DIVIDE)) {
    op    = token.type;
    token = next_token();

    factor();
    operate(op);
  }
}

static void factor(void) {
  switch (token.type) {
    case VARIABLE: {
      push(var[token.value]);
      break;
    }

    case INT: {
      push(token.value);
      break;
    }

    case LEFT_PAREN: {
      token = next_token();
      expression();
      has_error = check_token(RIGHT_PAREN);
      break;
    }

    default: {
      has_error = true;
      break;
    }
  }

  token = next_token();
}

Token next_token(void) {
  Token token = { OTHERS, 0 };

  while (isspace(ch)) {
    ch = next_char();
  }

  if (isdigit(ch)) {
    int num = 0;

    for (num = 0; isdigit(ch); ch = next_char()) {
      num = (num * 10) + (ch - '0');
    }

    token.type  = INT;
    token.value = num;
  } else if (islower(ch)) {
    token.type  = VARIABLE;
    token.value = ch - 'a';

    ch = next_char();
  } else {
    switch (ch) {
      case '(' : token.type = LEFT_PAREN;  break;
      case ')' : token.type = RIGHT_PAREN; break;
      case '+' : token.type = PLUS;        break;
      case '-' : token.type = MINUS;       break;
      case '*' : token.type = MULTI;       break;
      case '/' : token.type = DIVIDE;      break;
      case '=' : token.type = ASSIGN;      break;
      case '?' : token.type = PRINT;       break;
      case '\0': token.type = EOF_TOKEN;   break;
      default  : break;
    }

    ch = next_char();
  }

  return token;
}

static int next_char(void) {
  if (*pbuf == '\0') {
    return *pbuf;
  }

  return *pbuf++;
}

static void operate(TokenType op) {
  int op2 = pop();
  int op1 = pop();

  if ((op == DIVIDE) && (op2 == 0)) {
    fputs("Zero Division Error\n", stderr);
    has_error = true;
  }

  if (has_error) {
    return;
  }

  switch (op) {
    case PLUS  : push(op1 + op2); break;
    case MINUS : push(op1 - op2); break;
    case MULTI : push(op1 * op2); break;
    case DIVIDE: push(op1 / op2); break;
    default    : break;
  }
}

static void push(int n) {
  if (has_error) {
    return;
  }

  if (stack_pointer >= SIZE_OF_STACK) {
    fputs("Stack overflow\n", stderr);
    exit(EXIT_FAILURE);
  }

  stack[stack_pointer++] = n;
}

static int pop(void) {
  if (has_error) {
    return 1;
  }

  if (stack_pointer <= 0) {
    fputs("Stack is empty\n", stderr);
    exit(EXIT_FAILURE);
  }

  return stack[--stack_pointer];
}

static bool check_token(TokenType type) {
  return (bool)(token.type != type);
}
