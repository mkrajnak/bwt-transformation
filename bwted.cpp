#include "bwted.h"

void debug_alpha(unsigned char * alph){
  for (size_t i = 0; i < ALPHLENGTH; i++){
    cout << i << " " << alph[i] << "\t";
  }cout << endl;
}
bool str_sort(const char *c1, const char *c2){
    return strcmp(c1, c2) < 0;
}

int BWTEncoding(tBWTED *bwted, ifstream& inputFile, ofstream& outputFile){
  const uint32_t BUFFSIZE = 1000000;
  unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char)*BUFFSIZE);
  unsigned char alphabet[ALPHLENGTH];
  unsigned char tmpalpha[ALPHLENGTH];
  unsigned char* tmp = NULL;
  unsigned char* bwt_encoded = NULL;
  unsigned char* mtf_encoded = NULL;
  unsigned char tmp_c=0;
  for (size_t i = 0; i < ALPHLENGTH; i++)      // generate alphabet for MTF 
    alphabet[i] = char(i);
  
  while (inputFile.good())
  {
    inputFile.read((char *)buffer, BUFFSIZE);
    cout << "Read: " << inputFile.gcount() << endl;
    size_t lngth = inputFile.gcount();
    bwted->uncodedSize += lngth;
    
    //BWT encode
    tmp = (unsigned char*) malloc(lngth*sizeof(unsigned char));
    bwt_encoded = (unsigned char*) malloc(lngth*sizeof(unsigned char));
    vector<char *> permutations;
    buffer[lngth-1] = (unsigned char)0x03; 
    memcpy(tmp, buffer, lngth);
    memcpy(bwt_encoded, buffer, lngth);
    
    for (size_t i = 0; i < lngth; i++)// suffixes
    { 
      permutations.push_back((char*)&tmp[i]);
    }
    sort(permutations.begin(), permutations.end(), str_sort);
    for (size_t i = 0; i < lngth; i++) 
    { 
      bwt_encoded[i] = buffer[(lngth-(strlen(permutations[i]))-1 + lngth) % lngth];
    }
    // cout << "***" << endl;
    cout << bwt_encoded <<  endl;
    // MTF encode
    int alph_index;
    mtf_encoded = (unsigned char*) malloc(lngth*sizeof(unsigned char));
    char needle;
    // debug_alpha(alphabet);
    for (size_t i = 0; i < lngth; i++) 
    { 
      alph_index = 0;
      while(alph_index < ALPHLENGTH)
      {
        if ((unsigned char)bwt_encoded[i] == (unsigned char)alphabet[alph_index])
        {
          break;
        }
        ++alph_index;
      }
      mtf_encoded[i] = (u_char)alph_index;
      if (alph_index != 0)
      {
        needle = alphabet[alph_index];
        memcpy(tmpalpha, alphabet, ALPHLENGTH);
        memcpy(tmpalpha+1, alphabet, alph_index*sizeof(char));
        tmpalpha[0] = needle;
        memcpy(alphabet, tmpalpha, ALPHLENGTH);
      }
    }
    // RLE start
    list<char>rle(buffer, buffer+lngth);
    // list<char>rle(mtf_encoded, mtf_encoded+lngth);
    list<char>::iterator jt, it = rle.begin(); 
    size_t j =0;
    while ( j < rle.size())
    {
      tmp_c = *it;
      jt = it;
      int count = 0;
      while(tmp_c == *it && it != rle.end())  // aaaaa -> a4; aa->aa;
      {
        count++;
        ++it;
      }
      if(count > 2)
      {
        jt = rle.erase(jt,it);
        if(jt != rle.end()){
          rle.insert(jt,(char)tmp_c);
          rle.insert(jt,(char)26); 
          rle.insert(jt,(char)count-1); 
        }
        j = distance(rle.begin(), jt); 
      } else {
        j++;
        ++jt;
      }
      it = jt;
      // // cout << "8888888888" << endl;
      // for (auto a : rle)
      // {
      //   cout << "c: " << a << endl;
      // }
    }
    it = rle.begin();
    j=0;
    while(it != rle.end()) 
    {
      tmp_c = *it;
      cout <<"c: " << tmp_c << endl;
      outputFile.write((char*)&tmp_c, 1);
      ++it;
      j++;
    } 
   
    cout << "Written: " << j << endl;
    memset(tmp, 0, lngth);
    memset(bwt_encoded, 0, lngth);
    memset(mtf_encoded, 0, lngth);
    memset(buffer, 0, BUFFSIZE);
  }
  free(tmp);
  free(buffer);
  free(bwt_encoded);
  free(mtf_encoded);
  inputFile.close();
  outputFile.close();
  return 0;
}

typedef struct { // to remeber initial position of every character
  unsigned char c;
  size_t pos;
}decode;

bool cmpC(decode a, decode b){  // func required to sort the struct
  return (int)a.c < (int)b.c;
}

int BWTDecoding(tBWTED *bwted, ifstream& inputFile, ofstream& outputFile){
  cout << "Decoding" << endl;
  const uint32_t BUFFSIZE = 1000000;
  unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char)*BUFFSIZE);
  unsigned char alphabet[ALPHLENGTH];
  unsigned char tmpalpha[ALPHLENGTH];
  unsigned char* mtf_decoded = NULL;
  decode* tmp = NULL;
  unsigned char* bwt_decoded = NULL;
  for (size_t i = 0; i < ALPHLENGTH; i++)      // generate alphabet for MTF 
    alphabet[i] = (char)(i);

  inputFile.read((char*)buffer, BUFFSIZE);
  size_t lngth = inputFile.gcount();
  // for (size_t i = 0; i < lngth; i++)      // generate alphabet for MTF 
  //   cout << i << " : " << buffer[i] << endl; 
  // cout << "Read: " << inputFile.gcount() << endl;
  bwted->uncodedSize += lngth;
  // RLE decode
  list<char>rle(buffer, buffer+lngth);
  list<char>::iterator jt, it = rle.begin(); 
  
  size_t j=0;
  unsigned char tmp_c=0;
  while ( it != rle.end())
  {
    if((int)*it == 26)
    {
      it = rle.erase(it);
      int replace = (int)*it;
      it = rle.erase(it);
      rle.insert(it,replace,tmp_c);
      it = rle.begin();
      j+=replace-1;
      advance(it,j);
    }
    tmp_c = *it;
    ++it;
    ++j;
  }
  for (auto a : rle){
    cout << "c: "<< a << endl;
  }
  // MTF
  unsigned char* rle_decoded = (unsigned char*) malloc(rle.size());
  it = rle.begin();
  j=0;
  while(it != rle.end()) 
  {
    rle_decoded[j] = *it;
    ++it;
    j++;
  }
  cout << rle_decoded << endl; 
  // lngth = rle.size();
  mtf_decoded = (unsigned char*) malloc(lngth*sizeof(unsigned char));
  tmp = (decode*) malloc(lngth*sizeof(decode)); 
  for (size_t i = 0; i < lngth; i++) 
  { 
    // cout << alphabet[(int)buffer[i]] << endl;
    mtf_decoded[i] = alphabet[(int)buffer[i]];
    if ((int)buffer[i] != 0)
    {
      memcpy(tmpalpha, alphabet, ALPHLENGTH);
      memcpy(tmpalpha+1, alphabet, (int)buffer[i]);
      tmpalpha[0] = mtf_decoded[i];
      memcpy(alphabet, tmpalpha, ALPHLENGTH);
    }
  }
  for (size_t i = 0; i < lngth; i++) // bwt
  {
    tmp[i].c = (unsigned char)mtf_decoded[i];
    tmp[i].pos = i;
    // if (i == lngth-1) cout << "LOG" << i << endl;
  }
  stable_sort(tmp,tmp+lngth,cmpC);
  size_t tmp_pos = 0;
  bwt_decoded = (unsigned char*) malloc(lngth*sizeof(unsigned char));
  for (size_t i = 0; i < lngth; i++) 
  { 
    tmp_pos = tmp[tmp_pos].pos;
    bwt_decoded[i] = mtf_decoded[tmp_pos];
    if (bwt_decoded[i] != (int)0x03)
      outputFile.write((char*)&bwt_decoded[i],1);
  }
  outputFile.write("\0",1);
  cout << (char*)bwt_decoded  << endl; 
  memset(tmp, 0, lngth);
  memset(bwt_decoded, 0, lngth);
  memset(mtf_decoded, 0, lngth);
  memset(buffer, 0, BUFFSIZE);
  
  free(tmp);
  free(bwt_decoded);
  free(buffer);
  inputFile.close();
  outputFile.close();
  return 0;
}

