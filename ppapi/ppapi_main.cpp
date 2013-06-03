#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "ppapi_main/ppapi_main.h"
#include "ppb.h"

PPAPI_MAIN_WITH_DEFAULT_ARGS

int Maelstrom_main(int argc, const char* argv[]);

int ppapi_main(int argc, const char *argv[]) {
  if (!PPB_Init()) {
    fprintf(stderr, "PPB_Init failed!");
    return -1;
  }
  return Maelstrom_main(argc, argv);
}
