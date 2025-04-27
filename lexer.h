#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

/* simple token types */
typedef enum {
  /* types */
  INT,
  FLOAT,
  DOUBLE,
  LONG,
  CHAR,

  /* control statements */
  IF,
  ELSE,
  SWITCH,
  CASE,
  FOR,
  WHILE,
  DO,
  RETURN,

  /* comparison */
  EQUAL,
  N_EQUAL,
  OR,
  AND,
  G_T,
  L_T,
  GEQ_T,
  LEQ_T,

  /* ptr stuff */
  ACCESS,

  /* set */
  SET,
  
  /* literals*/
  CHAR_L,
  STRING_L,
  INT_L,
  DEC_L,

  /* delimiters */
  COMMA,
  SEMI_C,
  COLON,
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACK_C,
  RIGHT_BRACK_C,
  LEFT_BRACK_S,
  RIGHT_BRACK_S,

  /* operations */
  ADD,
  SUBTRC,
  INCR,
  DECR,
  MULT,
  DIVIDE,
  EQ_INCR,
  EQ_DECR,
  EQ_MULT,
  EQ_DIV,

  /* preprocessor */
  PND_INCL,
  PND_DEFN,
  INCL_LIB,

  /* other */
  TRUE,
  FALSE,

  IDENTIFIER
} token_type;

/* location struct */
typedef struct {
  size_t column;
  size_t line;
} token_loc;

/* actual token struct */
typedef struct {
  char *lexeme;
  token_loc coords;
  token_type type;
} token_t;


/* structure to hold the linked list of tokens */
typedef struct node{
  token_t *token;
  struct node *next;
} token_node;

typedef struct {
  char *word;
  token_type type;
} keyword_t;

/* look up table for keywords */
static keyword_t keywords[] = {
  {"int", INT},
  {"float", FLOAT},
  {"double", DOUBLE},
  {"long", LONG},
  {"char", CHAR},
  {"if", IF},
  {"else", ELSE},
  {"switch", SWITCH},
  {"case", CASE},
  {"for", FOR},
  {"while", WHILE},
  {"do", DO},
  {"return", RETURN},
  {"==", EQUAL},
  {"!=", N_EQUAL},
  {"||", OR},
  {"&&", AND},
  {">", G_T},
  {"<", L_T},
  {">=", GEQ_T},
  {"<=", LEQ_T},
  {"=", SET},
  {"+", ADD},
  {"-", SUBTRC},
  {"++", INCR},
  {"--", DECR},
  {"*", MULT},
  {"/", DIVIDE},
  {"+=", EQ_INCR},
  {"-=", EQ_DECR},
  {"*=", EQ_MULT},
  {"/=", EQ_DIV},
  {"->", ACCESS},
  {"#include", PND_INCL},
  {"#define", PND_DEFN},
  {"true", TRUE},
  {"false", FALSE}
};


/* function protoypes */
void lex(FILE *, token_node **);
bool tokenize(char *, int, int, int, token_node **);
void print_tokens(token_node **);
void create_token(token_type, int, int, char *, token_node **);
void add_list(token_t *, token_node **);
token_type find_type(char *);
void free_tokens(token_node **);

/* type checking functions */
bool is_delimiter(char);
bool is_string(char *);
bool is_char(char *);
bool is_numeric(char *);
bool is_dec(char *);
bool is_lib(char *);





#endif
