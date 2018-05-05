#ifndef BWTED_H
#define BWTED_H

#include "cstdint"
#include "cstdio"
#include "iostream"
#include "string.h"
#include "csignal"
#include "sstream"
#include "fstream"  
#include "vector"
#include "cstdbool"
#include "algorithm" 
#include "list"
using namespace std;

typedef struct{
  int64_t uncodedSize;
  int64_t codedSize;
}tBWTED;

const uint16_t ALPHLENGTH = 255;

/* bwted – záznam o kódování
inputFile – vstupní soubor (nekódovaný)
outputFile – výstupní soubor (kódovaný)
návratová hodnota – 0 kódování proběhlo v pořádku,-1 při kódování
nastala chyba */
int BWTEncoding(tBWTED *bwted, ifstream& inputFile, ofstream& outputFile);

/* bwted – záznam o dekódování
inputFile – vstupní soubor (kódovaný)
outputFile – výstupní soubor (dekódovaný)
návratová hodnota – 0 dekódování proběhlo v pořádku,-1 při dekódování
nastala chyba */
int BWTDecoding(tBWTED *bwted, ifstream& inputFile, ofstream& outputFile);

#endif
// debug output macro
#ifdef DEBUG
  #define dout cout
#else
  #define dout 0 && cout
#endif
