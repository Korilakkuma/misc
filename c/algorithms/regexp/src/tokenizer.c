#include "types.h"

void update_token(void) {
  if (*strbuf == '\0') {
    current_token = tk_end;
  } else {
    char c = *strbuf++;

    switch (c) {
      case '|': current_token = tk_union; break;
      case '(': current_token = tk_lpar;  break;
      case ')': current_token = tk_rpar;  break;
      case '*': current_token = tk_star;  break;
      case '+': current_token = tk_plus;  break;
      default:
        current_token = tk_char;
        token_char = c;
        break;
    }
  }
}
