#include "lexer.h"

#include <stdio.h>



int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Please specify a file to analyze\n");
    return 0;
  }

  FILE *fp = fopen(argv[1], "r");
  
  if (fp == NULL) {
    fprintf(stderr, "Error opening file!\n");
    return 0;
  }

  token_node *list = NULL;
  lex(fp, &list);
  print_tokens(&list);
  free_tokens(&list);
  return 0;
}
