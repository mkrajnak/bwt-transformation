#include "main.h"


void help(){
  printf("Burrows wheeler tranformation implemented by Martin Krajnak <xkrajn02@stud.fit.vutbr.cz>:\n"
        "Usage:\t  ./bwted -i <file> -o <file> -l <file> [-c|-x|-h]\n"
        "\t -i <file> name of input file, if not entered, input is taken from stdin\n"
        "\t -o <file> name of output file, if not entered, output is printed to stdout\n"
        "\t -l <file> log file, if not entered, logging is turned off\n"
        "\t -c compresion mode\n"
        "\t -x extraction mode\n"
        "\t -h print help and quit\n");
}

/**
* Eror message wrapper
*/
void err(string msg, int erno, short show_help){
  cerr << msg << endl;
  if (show_help) help();
  exit(erno);
}

/**
* Universal function for memory allocation checking
*/
void check_null(void *ptr){
  if (ptr == NULL) err("Memory allocation failure", ERR, 0);
}

void check_args(int argc, char **argv){
  ps=(params *)malloc (sizeof(params));
  check_null(ps);
  char c;
  while ((c = getopt(argc, argv, "i:o:l:cx")) != -1){
    switch (c) {
      case 'i':
        ps->inputFile = (char*) malloc(sizeof(optarg));
        check_null(ps->inputFile);
        strcpy(ps->inputFile, optarg);
        dout << "Input file: \t" << ps->inputFile << endl;
        break;
      case 'o':
        ps->outputFile = (char*) malloc(sizeof(optarg));
        check_null(ps->outputFile);
        strcpy(ps->outputFile, optarg);
        dout << "Output file: \t" << ps->outputFile << endl;
        break;
      case 'l':
        ps->logFile = (char*) malloc(sizeof(optarg));
        check_null(ps->logFile);
        strcpy(ps->logFile, optarg);
        dout << "Log file: \t" << ps->logFile << endl;
        break;
      case 'c':
        ps->compress = true;
        dout << "Compressing" << endl;
        break;
      case 'x':
        ps->compress = false;
        dout << "Decompressing" << endl;
        break;
      case '?':
        help();
        break;
      default:
        break;
    }
  }
}

void cleanup(int sig) {
  if (ps->inputFile != NULL)      free(ps->inputFile);
  if (ps->outputFile != NULL)     free(ps->outputFile);
  if (ps->logFile != NULL)        free(ps->logFile);
  if (ps != NULL)                 free(ps);
  signal(sig, SIG_IGN);
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) 
{
  signal(SIGINT, cleanup);
  check_args(argc, argv);

  tBWTED *BWT;
 
  ifstream ifs (ps->inputFile, ifstream::binary);
  ifstream ofs (ps->outputFile, ifstream::out);
  
  if (ps->compress)
    BWTEncoding(BWT, ifs, ofs);
  else 
    BWTDecoding(BWT, ifs, ofs);

  cleanup(0);
  return 0;
}
