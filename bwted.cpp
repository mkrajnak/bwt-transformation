#include "bwted.h"


void debug_alpha(char * alph){
  for (size_t i = 0; i < ALPHLENGTH; i++){
    cout << i << " " << alph[i] << "\t";
  }cout << endl;
}

int BWTEncoding(tBWTED *bwted, ifstream& inputFile, ofstream& outputFile){
  const uint32_t BUFFSIZE = 8196;
  char buffer[BUFFSIZE];
  char alphabet[ALPHLENGTH];
  char tmpalpha[ALPHLENGTH];
  char* tmp = (char*) malloc(BUFFSIZE+1);
  char* bwt_encoded = (char*) malloc(BUFFSIZE+1);
  char* mtf_encoded = (char*) malloc(BUFFSIZE+1);

  for (size_t i = 0; i < ALPHLENGTH; i++)      // generate alphabet for MTF 
    alphabet[i] = char(i);
  
  while (inputFile.good())
  {
    inputFile.read(buffer, BUFFSIZE);
    cout << buffer << endl;
    cout << "Read: " << inputFile.gcount() << endl;
    size_t lngth = inputFile.gcount();
    bwted->uncodedSize += lngth;
    
    //BWT encode
    tmp =  (char*) malloc(lngth);
    bwt_encoded = (char*) malloc(lngth);
    vector<string> permutations;
    buffer[lngth-1] = (char)0x03; 
    memcpy(tmp, buffer, lngth);
    memcpy(bwt_encoded, buffer, lngth);
    
    for (size_t i = 0; i < lngth; i++) 
    { 
      cout << i << ":"<<buffer[i] << endl;
      permutations.push_back(&tmp[i]);
    }
    sort(permutations.begin(), permutations.end());
    for (size_t i = 0; i < lngth; i++) 
    { 
      bwt_encoded[i] = buffer[(lngth-(permutations[i].length())-1 + lngth) % lngth];
    }
    // cout << "***" << endl;
    cout << bwt_encoded <<  endl;
    // MTF encode
    int alph_index;
    char* mtf_encoded = (char*) malloc(lngth);
    char needle;
    // debug_alpha(alphabet);
    for (size_t i = 0; i < lngth; i++) 
    { 
      alph_index = 0;
      while(alph_index < ALPHLENGTH)
      {
        if ((char)bwt_encoded[i] == (char)alphabet[alph_index])
        {
          break;
        }
        ++alph_index;
      }
      mtf_encoded[i] =(char)alph_index;
      outputFile.write(&mtf_encoded[i], 1);
      if (alph_index != 0)
      {
        needle = alphabet[alph_index];
        memcpy(tmpalpha, alphabet, ALPHLENGTH);
        memcpy(tmpalpha+1, alphabet, alph_index*sizeof(char));
        tmpalpha[0] = needle;
        memcpy(alphabet, tmpalpha, ALPHLENGTH);
      }
    }
    cout << "Written: " << lngth << endl;
    memset(tmp, 0, lngth);
    memset(bwt_encoded, 0, lngth);
    memset(mtf_encoded, 0, lngth);
    memset(buffer, 0, BUFFSIZE);
  }
  free(tmp);
  free(bwt_encoded);
  free(mtf_encoded);
  inputFile.close();
  outputFile.close();
  return 0;
}

typedef struct { // to remeber initial position of every character
  char c;
  size_t pos;
}decode;

bool cmpC(decode a, decode b){  // func required to sort the struct
  return a.c < b.c;
}

int BWTDecoding(tBWTED *bwted, ifstream& inputFile, ofstream& outputFile){
  // cout << "Decoding" << endl;
  const uint32_t BUFFSIZE = 8197;
  char buffer[BUFFSIZE];
  char alphabet[ALPHLENGTH];
  char tmpalpha[ALPHLENGTH];
  decode* tmp = (decode*) malloc(BUFFSIZE*sizeof(decode)); 
  char* bwt_decoded = (char*) malloc(BUFFSIZE);
  char* mtf_decoded = (char*) malloc(BUFFSIZE);

  for (size_t i = 0; i < ALPHLENGTH; i++)      // generate alphabet for MTF 
    alphabet[i] = char(i);

  while (inputFile.good())
  {
    inputFile.read(buffer, BUFFSIZE);
    
    // cout << "Read: " << inputFile.gcount() << endl;
    size_t lngth = inputFile.gcount();
    bwted->uncodedSize += lngth;
    
    for (size_t i = 0; i < lngth; i++) 
    { 
      // cout << alphabet[(int)buffer[i]] << endl;
      mtf_decoded[i] = alphabet[(int)buffer[i]];
      if ((int)buffer[i] != 0)
      {
        memcpy(tmpalpha, alphabet, ALPHLENGTH);
        memcpy(tmpalpha+1, alphabet, (int)buffer[i]*sizeof(char));
        tmpalpha[0] = mtf_decoded[i];
        memcpy(alphabet, tmpalpha, ALPHLENGTH);
      }
    }
    for (size_t i = 0; i < lngth; i++) 
    {
      tmp[i].c = mtf_decoded[i];
      tmp[i].pos = i;
    }
    stable_sort(tmp,tmp+lngth,cmpC);
    size_t tmp_pos = 0;
    for (size_t i = 0; i < lngth; i++) 
    { 
      tmp_pos = tmp[tmp_pos].pos;
      bwt_decoded[i] = mtf_decoded[tmp_pos];
      if (bwt_decoded[i] != (int)0x03)
        outputFile.write(&bwt_decoded[i],1);
      cout << i << ":"<<bwt_decoded[i] << endl;
      
    }
    memset(tmp, 0, lngth);
    memset(bwt_decoded, 0, lngth);
    memset(mtf_decoded, 0, lngth);
    memset(buffer, 0, BUFFSIZE);
  }
  free(mtf_decoded);
  free(tmp);
  free(bwt_decoded);
  inputFile.close();
  outputFile.close();
  return 0;
}

