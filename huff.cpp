#include "huff.h"


using namespace std;

int main(int argc, char *argv[]){
  compress comp;

  if(argc != 2){
    cerr << "Not the correct input \n";
    return 1;
  }
  if(!comp.fileOpen(argv[1])){
    cerr << "Failed to open file \n";
    return 1;
  }


  comp.weights();
  comp.tWeights();
  return 0;
}

compress::compress(){
     for(int i = 0; i < 256; i++){weight[i] = 0;}
}

compress::~compress(){
   if(file != NULL)
     file.close();
}

bool compress::fileOpen(char *fileName){
  file.open(fileName, fstream::in);
  if(file == NULL){
    return 0;
  }
  return 1;
}

void compress::weights(){
  char readChar;
  file.seekg(file.beg);

  while(file.get(readChar)){
    cout << readChar << " " ;
    ++weight[(int)readChar];
  }
  cout << endl;
}


void compress::tWeights(){
  for(int i = 0; i<256;i++){
    if(weight[i] != 0){
      cout << "Character: " <<(char) i << " Weigth: " << weight[i] << endl;
    }
  }
}
