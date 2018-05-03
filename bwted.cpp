#include "bwted.h"

int BWTEncoding(tBWTED *bwted, ifstream& inputFile, ifstream& outputFile){
  
  const uint16_t BUFFSIZE = 8192;
  char buffer[BUFFSIZE];
  
//   while (ifs.good()) {
  
  inputFile.read(buffer, BUFFSIZE);
  inputFile.close();
  cout << "Read: " << inputFile.gcount() << endl;
  
  size_t lngth = inputFile.gcount();
  cout << "Length: " << lngth << endl;
  char* tmp =  (char*) malloc(lngth);
  char* bwt_encoded = (char*) malloc(lngth);
  int original = 0;
  vector<string> permutations;
  buffer[lngth-1] = '$'; 
  memcpy(tmp, buffer, lngth);
  char last;
  for (size_t i = 0; i < lngth; i++) 
  { 
    permutations.push_back(&tmp[i]);
  }
  sort(permutations.begin(), permutations.end());
  string result = "";
  for (size_t i = 0; i < lngth; i++) 
  { 
    bwt_encoded[i] = buffer[(lngth-(permutations[i].length())-1 + lngth) % lngth];
    if(permutations[i].length() == lngth)
      original = i;
  }
  cout << "***" << endl;
  cout << bwt_encoded << "\t:" << original <<  endl;
  free(tmp);
  free(bwt_encoded);
  return 0;
}

typedef struct { // to remeber initial position of every character
  char c;
  size_t pos;
}decode;

bool cmpC(decode a, decode b){  // func required to sort the struct
  return a.c < b.c;
}

int BWTDecoding(tBWTED *bwted, ifstream& inputFile, ifstream& outputFile){
  cout << "Decoding" << endl;
  const uint16_t BUFFSIZE = 8192;
  char buffer[BUFFSIZE];
  
  inputFile.read(buffer, BUFFSIZE);
  inputFile.close();
  cout << "Read: " << inputFile.gcount() << endl;

  size_t lngth = inputFile.gcount()-1;
  cout << "Length: " << lngth << endl;
  // buffer[lngth] = '\0';
  decode* tmp = (decode*) malloc(lngth*sizeof(decode));
  char* result =  (char*) malloc(lngth);
  // cout << tmp << endl;z
  
  for (size_t i = 0; i < lngth; i++) 
  {
    tmp[i].c = buffer[i];
    tmp[i].pos = i;
  }
  cout << endl;
  stable_sort(tmp,tmp+lngth,cmpC);
  size_t tmp_pos = 0;
  for (size_t i = 0; i < lngth; i++) 
  { 
    tmp_pos = tmp[tmp_pos].pos;
    result[i] = buffer[tmp_pos];
  }
  cout << result << endl;
  free(tmp);
  free(result);
  return 0;
}

