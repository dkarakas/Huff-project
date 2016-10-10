#include "huff.h"


using namespace std;

int main(int argc, char *argv[]){
  fstream FileToComp;
  compress comp;
  if(argc != 2){
    cerr << "Not the correct input \n";
    return 1;
  }


  FileToComp.open(argv[1], fstream::in);
  if(FileToComp == NULL){
    cerr << "Failed to open file \n";
    return 1;
  }

  comp.file = FileToComp;
  comp.weights();
  comp.tweights();

  FileToComp.close();
  return 0;
}


void comp::weigts(){
  char readChar;
  file.seekg(file.beg)

  while(file.get(readChar)){
    cout << readChar;
    ++weights[readChar];
  }
}


void comp::tweigts(){
  for(int i = 0; i<256;i++){
    if(weights[i] != 0){
      cout << "Character: " <<(char) i << " Weigth: " << weights[i] << endl;
    }
  }
}
