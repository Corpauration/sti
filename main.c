#include "stasm.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *fs;

  if (argc == 2) {

    fs = fopen(argv[1], "r");

    if (fs == NULL) {
      fprintf(stderr, "Can't open file : %s", argv[1]);
      exit(EXIT_FAILURE);
    }
    
    run_from_fs(fs);
    fclose(fs);
  }

  return 0;
}
