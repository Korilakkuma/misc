typedef enum {
  tk_char,
  tk_union,
  tk_lpar,
  tk_rpar,
  tk_star,
  tk_plus,
  tk_end
} token_t;

char token_char;
token_t current_token;
char *strbuf;

extern void update_token(void);
