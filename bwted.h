#ifndef BWTED_H
#define BWTED_H

#include "cstdint"
#include "cstdio"

typedef struct{
  int64_t uncodedSize;
  int64_t codedSize;
}tBWTED;

/* bwted – záznam o kódování
inputFile – vstupní soubor (nekódovaný)
outputFile – výstupní soubor (kódovaný)
návratová hodnota – 0 kódování proběhlo v pořádku,-1 při kódování
nastala chyba */
int BWTEncoding(tBWTED *bwted, FILE *inputFile, FILE *outputFile);

/* bwted – záznam o dekódování
inputFile – vstupní soubor (kódovaný)
outputFile – výstupní soubor (dekódovaný)
návratová hodnota – 0 dekódování proběhlo v pořádku,-1 při dekódování
nastala chyba */
int BWTDecoding(tBWTED *ahed, FILE *inputFile, FILE *outputFile);

#endif
