#include "bwted.h"


void debug_alpha(char * alph){
  for (size_t i = 0; i < ALPHLENGTH; i++){
    cout << i << " " << alph[i] << "\t";
  }cout << endl;
}

int BWTEncoding(tBWTED *bwted, ifstream& inputFile, ifstream& outputFile){
   
  const uint16_t BUFFSIZE = 8192;
  char buffer[BUFFSIZE];
  char alphabet[ALPHLENGTH];
  char tmpalpha[ALPHLENGTH];
  for (size_t i = 0; i < ALPHLENGTH; i++)      // generate alphabet for MTF 
    alphabet[i] = char(i);
  
//   while (ifs.good()) {
  
  inputFile.read(buffer, BUFFSIZE);
  cout << "Read: " << inputFile.gcount() << endl;
  
  size_t lngth = inputFile.gcount()+1;
  cout << "Length: " << lngth << endl;
  //BWT encode
  char* tmp =  (char*) malloc(lngth);
  char* bwt_encoded = (char*) malloc(lngth);
  vector<string> permutations;
  buffer[lngth-1] = '$'; 
  memcpy(tmp, buffer, lngth);
  memcpy(bwt_encoded, buffer, lngth);
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
  }
  cout << "***" << endl;
  cout << bwt_encoded << "\t:" <<  endl;
  // MTF encode
  int alph_index;
  int* mtf_encoded = (int*) malloc(lngth);
  char needle;
  // debug_alpha(alphabet);
  for (size_t i = 0; i < lngth; i++) 
  { 
    alph_index = 0;
    while(alph_index < ALPHLENGTH)
    {
      if ((char)bwt_encoded[i] == (char)alphabet[alph_index])
      {
        cout << "Found " << (char)bwt_encoded[i] << " aka " << (char)alphabet[alph_index] <<" at pos " << alph_index <<endl;
        break;
      }
      ++alph_index;
    }
    mtf_encoded[i] = alph_index;
    if (alph_index != 0)
    {
      needle = alphabet[alph_index];
      memcpy(tmpalpha, alphabet, ALPHLENGTH);
      memcpy(tmpalpha+1, alphabet, alph_index*sizeof(char));
      tmpalpha[0] = needle;
      memcpy(alphabet, tmpalpha, ALPHLENGTH);
    }
  }
  for (size_t i = 0; i < lngth; i++) 
  {
    cout << mtf_encoded[i] << " ";
  }
  cout << endl;
  
  free(tmp);
  free(bwt_encoded);
  free(mtf_encoded);
  inputFile.close();
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

