#include "bwted.h"


void debug_alpha(char * alph){
  for (size_t i = 0; i < ALPHLENGTH; i++){
    cout << i << " " << alph[i] << "\t";
  }cout << endl;
}

int BWTEncoding(tBWTED *bwted, ifstream& inputFile, ofstream& outputFile){
  const uint32_t BUFFSIZE = 100;
  char buffer[BUFFSIZE];
  char alphabet[ALPHLENGTH];
  char tmpalpha[ALPHLENGTH];
  char* tmp;
  char* bwt_encoded;
  char* mtf_encoded;
  char tmp_c=0;
  for (size_t i = 0; i < ALPHLENGTH; i++)      // generate alphabet for MTF 
    alphabet[i] = char(i);
  
  while (inputFile.good())
  {
    inputFile.read(buffer, BUFFSIZE);
    cout << "Read: " << inputFile.gcount() << endl;
    size_t lngth = inputFile.gcount();
    bwted->uncodedSize += lngth;
    
    //BWT encode
    tmp = (char*) malloc(lngth);
    bwt_encoded = (char*) malloc(lngth);
    vector<string> permutations;
    buffer[lngth-1] = (char)0x03; 
    memcpy(tmp, buffer, lngth);
    memcpy(bwt_encoded, buffer, lngth);
    
    for (size_t i = 0; i < lngth; i++)// suffixes
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
    mtf_encoded = (char*) malloc(lngth);
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
    outputFile.write("\0", 1);
    // RLE start
    // list<char>rle(mtf_encoded, mtf_encoded+lngth);
    // list<char>::iterator jt, it = rle.begin(); 
    // size_t j =0;
    // while ( j < rle.size())
    // {
    //   cout << "exec " << j << " " << *it << endl ;
    //   if((int)*it <= 58 && (int)*it >= 48)    // actual number are marked as numbers
    //   {
    //     if(it != rle.end()){
    //       rle.insert(it,(char)'@'); 
    //     } else {
    //       rle.push_back((char)'@');
    //       break;
    //     }
    //     it = rle.begin();
    //     cout << "Pointing to " << *it << endl;
    //     advance(it,j+1);
    //     j +=2;
    //     cout << "Pointing to " << *it << endl;
    //     ++it;
    //     continue;
    //   }
    //   tmp_c = *it;
    //   jt = it;
    //   int count = -1;
    //   while(tmp_c == *it && it != rle.end())  // aaaaa -> a4; aa->aa;
    //   {
    //     count++;
    //     ++it;
    //   }
    //   if(count >1)
    //   {
    //     cout << "c " << count <<endl;
    //     jt = rle.erase(jt,it);
    //     if(jt != rle.end()){
    //       rle.insert(jt,(char)tmp_c); 
    //       rle.insert(jt,(char)count); 
    //     } else {
    //       rle.push_back((char)count);
    //       break;
    //     }
    //     j = distance(rle.begin(), jt); 
    //   } else {
    //     j++;
    //     ++jt;
    //   }
    //   it = jt;
    // }
    // it = rle.begin();
    // j=0;
    // while(it != rle.end()) 
    // {
    //   tmp_c = *it;
    //   outputFile.write(&tmp_c, 1);
    //   ++it;
    // } 
   
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
  return (int)a.c < (int)b.c;
}

int BWTDecoding(tBWTED *bwted, ifstream& inputFile, ofstream& outputFile){
  // cout << "Decoding" << endl;
  const uint32_t BUFFSIZE = 8196;
  char buffer[BUFFSIZE];
  char alphabet[ALPHLENGTH];
  char tmpalpha[ALPHLENGTH];
  decode* tmp = (decode*) malloc(BUFFSIZE*sizeof(decode)); 
  char* bwt_decoded = (char*) malloc(BUFFSIZE);

  for (size_t i = 0; i < ALPHLENGTH; i++)      // generate alphabet for MTF 
    alphabet[i] = char(i);

  while (inputFile.good())
  {
    inputFile.read(buffer, BUFFSIZE);
    
    size_t lngth = inputFile.gcount()-1;
    cout << "Read: " << inputFile.gcount() << endl;
    bwted->uncodedSize += lngth;
    // RLE decode
    // list<char>rle(buffer, buffer+lngth);
    // list<char>::iterator jt, it = rle.begin(); 
    
    // size_t j=0;
    // char tmp_c=0;
    // while ( it != rle.end())
    // {
    //   cout << "c: "<< *it << endl;
    //   if(*it == '@')
    //   {
    //     it = rle.erase(it);
    //   }
    //   else if((int)*it <= 58 && (int)*it >= 48)    // actual number are marked as numbers
    //   {
    //     int replace = *it - '0'-1;
    //     it = rle.erase(it);
    //     rle.insert(it,replace,tmp_c);
    //     it = rle.begin();
    //     j+=replace-1;
    //     advance(it,j);
    //     cout << j << endl;    
    //   }
    //   tmp_c = *it;
    //   ++it;
    //   ++j;
    // }
    // // cout << "***" << endl;
    // // for (auto a : rle ) 
    // // { 
    // //   cout << a << endl;
    // // }
    // // MTF
    // char* rle_decoded = (char*) malloc(rle.size());
    
    // it = rle.begin();
    // j=0;
    // while(it != rle.end()) 
    // {
    //   rle_decoded[j] = *it;
    //   ++it;
    //   j++;
    // } 
    // lngth = rle.size();
    char* mtf_decoded = (char*) malloc(lngth);
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
    }
    outputFile.write("\0",1);
    cout << bwt_decoded  << endl; 
    memset(tmp, 0, lngth);
    memset(bwt_decoded, 0, lngth);
    memset(mtf_decoded, 0, lngth);
    memset(buffer, 0, BUFFSIZE);
  }
  free(tmp);
  free(bwt_decoded);
  inputFile.close();
  outputFile.close();
  return 0;
}

