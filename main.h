#ifndef MAIN_H
#define MAIN_H

#include "cstdbool"
#include "cstring"
#include "bwted.h"
#include "getopt.h"
#include "iostream"
#include "string.h"
#include "csignal"

typedef struct{
  char *inputFile;
  char *outputFile;
  char *logFile;
  bool compress;
}params;

using namespace std;

#define ERR -1

params *ps;
#endif

// debug output macro
#ifdef DEBUG
  #define dout cout
#else
  #define dout 0 && cout
#endif
