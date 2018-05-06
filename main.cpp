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
  ps->inputFile = NULL;
  ps->outputFile = NULL;
  ps->logFile = NULL;
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
  
  tBWTED *bwted =(tBWTED *) malloc(sizeof(tBWTED));
  check_null(bwted);
  bwted->codedSize = 0;
  bwted->uncodedSize = 0;
 
  ifstream ifs (ps->inputFile, ifstream::binary);
  ofstream ofs (ps->outputFile, ofstream::binary);
  

  if (ps->compress)
    BWTEncoding(bwted, ifs, ofs);
  else 
    BWTDecoding(bwted, ifs, ofs);
  
  if(ps->logFile){
    ofstream lfl (ps->logFile, ofstream::out);
    if(ps->compress)
      lfl.write("Encoding\n",strlen("Encoding\n"));
    else
      lfl.write("Decoding\n",strlen("Decoding\n"));
    
    lfl.write("Read ",strlen("Read "));
    string tmp = to_string(bwted->uncodedSize);
    lfl.write(tmp.c_str(),sizeof(tmp.c_str()));
    lfl.write("\n",strlen("\n"));
    
    lfl.write("Written ",strlen("Written "));
    string tmp2 = to_string(bwted->codedSize);
    lfl.write(tmp2.c_str(),sizeof(tmp2.c_str()));
    lfl.write("\n",strlen("\n"));
    lfl.close();
  }

  cleanup(0);
  return 0;
}
