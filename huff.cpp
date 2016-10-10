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






//COMPRESS CLASSS

compress::compress(){
     //initilize the weight array
     for(int i = 0; i < 256; i++){weight[i] = 0;}
}

compress::~compress(){
   //closes the file at the end
   if(file != NULL)
     file.close();
}

bool compress::fileOpen(char *fileName){
  //opens the file and if it is false the program gives an error
  file.open(fileName, fstream::in);
  if(file == NULL){
    return 0;
  }
  return 1;
}

void compress::weights(){
  //finds the weights of all the chars
  char readChar;
  file.seekg(file.beg);

  while(file.get(readChar)){
    cout << readChar << " " ;
    ++weight[(int)readChar];
  }
  cout << endl;
}


void compress::tWeights(){
  //prints for test purposes;
  for(int i = 0; i<256;i++){
    if(weight[i] != 0){
      cout << "Character: " <<(char) i << " Weigth: " << weight[i] << endl;
    }
  }
}



//NODE CLASS
node::node(char infoChar,long int weightCalc){ 
  left = NULL;
  right = NULL;
  next = NULL;
  info = infoChar;
  weight = weightCalc;
}




