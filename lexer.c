#include "lexer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void lex(FILE *fp, token_node **list) {
  assert(fp != NULL && list != NULL);
  int line = 1;
  int col = 1;
  int save_col = 1;
  while (1) {
    bool end = false;
    bool is_string = false;
    bool is_char = false;
    bool is_incr = false;
    int index = 0;
    save_col = col;
    char buffer[256] = {0};
    char c1 = fgetc(fp);
    if (c1 == '\n') {
      line++;
      col = 1;
      continue;
    }
    if (c1 == EOF) {
      break;
    }
    if (c1 == ' ') {
      col++;
      continue;
    }
    if (c1 == '\"') {
      is_string = true;
    }
    if (c1 == '\'') {
      is_char = true;
    }
    if (c1 == '+' || c1 == '-') {
      is_incr = true;
    }
    buffer[index] = c1;
    index++;

    //now we have the first character, check if it is tokenizeable
    //else we keep adding a char until we create a token
    if (!tokenize(buffer, line, save_col, index, list)) {
      while (1) {
        if (index > 255) {
          fprintf(stderr, "lex string too long!\n");
          exit(1);
        }

        char c2 = fgetc(fp);
        if (c2 != '+' && c2 != '-') {
          is_incr = false;
        }
        if (c2 == '\'') {
          is_char = false;
        }
        if (c2 != EOF && !(is_delimiter(c2)) || is_string || is_incr || is_char) {
          buffer[index] = c2;
          index++;
          col++;
        }
        if ((!is_string && !is_incr && !is_char && (c2 == EOF || is_delimiter(c2))) || (is_string && buffer[index - 1] == '\"')) {
          //special case for identifiers of length 1
          buffer[index] = '\0';
          if (index == 1) {
            tokenize(buffer, line, save_col, 2, list);
          }
          else {
            tokenize(buffer, line, save_col, index, list);
          }
          if (!is_string && !is_char) {
            fseek(fp, -1, SEEK_CUR);
          }
          col++;
          break;
        }
        if (c2 == EOF) {
          end = true;
          break;
        }
      }


    }
    else {
      col++;
    }
    if (end) {
      break;
    }
  }
  printf("Finished lexing!\n");
}


bool tokenize(char *buffer, int line, int col, int length, token_node **list) {
  if (length == 1) {
    char c = buffer[0];
    switch (c) {
      case (','):
        create_token(COMMA, line, col, buffer, list);
        break;
      case (';'):
        create_token(COLON, line, col, buffer, list);
        break;
      case ('('):
        create_token(LEFT_PAREN, line, col, buffer, list);
        break;
      case (')'):
        create_token(RIGHT_PAREN, line, col, buffer, list);
        break;
      case ('{'):
        create_token(LEFT_BRACK_C, line, col, buffer, list);
        break;
      case ('}'):
        create_token(RIGHT_BRACK_C, line, col, buffer, list);
        break;
      case (':'):
        create_token(COLON, line, col, buffer, list);
        break;
      case ('['):
          create_token(LEFT_BRACK_S, line, col, buffer, list);
        break;
      case (']'):
        create_token(RIGHT_BRACK_S, line, col, buffer, list);
        break;
      default:
        return false;
    }
    return true;
  }
  else {
    if (is_char(buffer)) {
      create_token(CHAR_L, line, col, buffer, list);
    }
    else if (is_string(buffer)) {
      create_token(STRING_L, line, col, buffer, list);
    }
    else if (is_numeric(buffer)) {
      create_token(INT_L, line, col, buffer, list);
    }
    else if (is_dec(buffer)) {
      create_token(DEC_L, line, col, buffer, list);
    }
    else if (is_lib(buffer)) {
      create_token(INCL_LIB, line, col, buffer, list);
    }
    else {
      token_type type = find_type(buffer);
      create_token(type, line, col, buffer, list);
    }
    return true;
  }
}


void create_token(token_type type, int line, int col, char *lexeme, token_node **list) {
  token_t *new_token = malloc(sizeof(token_t));
  assert(new_token != NULL);
  new_token->type = type;
  new_token->coords.line = line;
  new_token->coords.column = col;
  new_token->lexeme = malloc(strlen(lexeme) + 1);
  strcpy(new_token->lexeme, lexeme);
  add_list(new_token, list);
 }

void add_list(token_t *token, token_node **list) {
  token_node *new_node = malloc(sizeof(token_node));
  assert(new_node != NULL);
  new_node->token = token;
  new_node->next = NULL;

  token_node *curr = *list;
  if (curr == NULL) {
    *list = new_node;
    return;
  }
  else {
    while (curr->next != NULL) {
      curr = curr->next;
    }
  }
  curr->next = new_node;
}

void print_tokens(token_node **list) {
  token_node *curr = *list;
  while (curr != NULL) {
    token_t *curr_token = curr->token;
    printf("Token Type: %d | Lexeme: %s | Line: %zu | Col: %zu\n", curr_token->type, curr_token->lexeme, curr_token->coords.line, curr_token->coords.column);
    curr = curr->next;
  }
}

bool is_delimiter(char c) {
  const char delims[] = {'\n', ' ', ',', '(', ')', '{', '}', ';', ':', '[', ']', '+', '-'};
  for (int i = 0; i < sizeof(delims); i++) {
    if (c == delims[i]) {
      return true;
    }
  }
  return false;
}


bool is_string(char *string) {
  int length = strlen(string);
  if (string[0] == '"' && string[length - 1] == '"') return true;
  return false;
}

bool is_char(char *string) {
  int length = strlen(string);
  if (string[0] == '\'' && string[length - 1] == '\'') return true;
  return false;
}

bool is_numeric(char *string) {
  int length = strlen(string);
  for (int i = 0; i < length; i++) {
    if (!isdigit(string[i])) {
      return false;
    }
  }
  return true;
}

bool is_dec(char *string) {
  int length = strlen(string);
  int cnt = 0;
  for (int i = 0; i < length; i++) {
    if (!isdigit(string[i])) {
      if (string[i] == '.') {
        cnt++;
      }
      else {
        return false;
      }
    }
  }
  if (cnt > 1) {
    return false;
  }
  return true;
}

bool is_lib(char *string) {
  int length = strlen(string);
  if (string[0] == '<' && string[length - 1] == '>') {
    return true;
  }
  return false;
}

token_type find_type(char *string) {
  for (int i = 0; i < sizeof(keywords) / sizeof(keyword_t); i++) {
    if (strcmp(string, keywords[i].word) == 0) {
      return keywords[i].type;
    }
  }
  return IDENTIFIER;
}

void free_tokens(token_node **list) {
  token_node *our_token = *list;
  while (our_token != NULL) {
    token_node *next_tok = our_token->next;
    token_t *token = our_token->token;
    free(token->lexeme);
    free(token);
    free(our_token);
    our_token = next_tok;
  }
}

