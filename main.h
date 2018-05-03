#ifndef MAIN_H
#define MAIN_H

#include "cstring"
#include "bwted.h"
#include "getopt.h"
using namespace std;

typedef struct{
  char *inputFile;
  char *outputFile;
  char *logFile;
  bool compress;
}params;


#define ERR -1

params *ps;
#endif
