#include "huff.h"

#define  CHAR_SIZE 8

using namespace std;

int main(int argc, char *argv[]){
  compress comp;
  double cend,cstart;

  if(argc != 2){
    cerr << "Not the correct input \n";
    return 1;
  }
  if(!comp.fileOpen(argv[1])){
    cerr << "Failed to open file2 \n";
    return 1;
  }
  cstart = (double) clock();
  comp.weights();
  comp.createPriorityQueue();
  comp.enqueuePrior(1,200);
  comp.printQueue();
  comp.constructTree();
  comp.preTravTree(comp.getHead());
  comp.saveTree(comp.getHead());
  comp.emptyW();
  comp.createTable(comp.getHead());
  //work until here
  comp.writeFile();
  comp.emptyWEOF();
  comp.emptyW();
  cend = (double) clock();
  cout << (cend - cstart)/CLOCKS_PER_SEC;
  cout << endl;
  
  //comp.tWeights();
  return 0;
}


//COMPRESS CLASSS

compress::compress(){
     
   //initilize the weight array
   for(int i = 0; i < 256; i++){weight[i] = 0;}
    
   priority_Head = NULL;
   char_to_be_written = 0x00;
   int_to_be_written = 0;
   num_written_char = 0;
}

compress::~compress(){
   //closes the file at the end
   if(file != NULL)
     file.close();
   if(fileToOutput != NULL)
     fileToOutput.close();
}

bool compress::fileOpen(char *fileName){
  std::string file_s(fileName);
  std::string huff(".huff");
  //opens the file and if it is false the program gives an error
  file.open(fileName, fstream::in);
  if(!file.is_open() ){
    return 0;
  }
  
  file_s = file_s + huff;
  fileToOutput.open(file_s.c_str(), fstream::out);
  if(!fileToOutput.is_open()){
    return 0;
  }
  return 1;
}

void compress::weights(){
  //finds the weights of all the chars
  char readChar;
  file.seekg(file.beg);

  while(file.get(readChar)){
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

node *compress::createNode(char infoChar, long int weightCalc){
  node *newNode = new node; 

  if(newNode == NULL){
    cerr << "Error allocating space";
    return NULL;
  }

  newNode->left = NULL;
  newNode->right = NULL;
  newNode->next = NULL;
  newNode->info = infoChar;
  newNode->weight = weightCalc;
  return newNode;
}

void compress::createPriorityQueue(){
  for(int i = 0; i < 256; i++){
    if(weight[i] != 0){
      enqueuePrior(weight[i],(char)i);
    }
  }
}

void compress::enqueuePrior(long int weight,char info){
  node* new_Node_Prior = createNode(info,weight);
  node * cur = NULL;
  node * prev  = NULL;
  node dummy;


  if(new_Node_Prior == NULL){
    cerr << "Error allocating memory";
    return;
  }

  dummy.next = priority_Head;
  cur = priority_Head;
  prev = &dummy;
  while(cur != NULL){
    if(new_Node_Prior->weight <= cur->weight){
       break;
    }
    else{
      prev = cur;
      cur = cur->next;
    }
  }
  prev->next = new_Node_Prior;
  new_Node_Prior->next = cur;
  priority_Head = dummy.next;
}

void compress::printQueue(){
  node *cur = priority_Head;
  while(cur != NULL){
    cout << cur->info << " " << cur->weight << endl;
    cur = cur->next;
  }
}

void compress::enqueueTree(node * new_Node_Prior ){
  node * cur = NULL;
  node * prev  = NULL;
  node dummy;

  if(new_Node_Prior == NULL){
    cerr << "Error allocating memory";
    return;
  }

  dummy.next = priority_Head;
  cur = priority_Head;
  prev = &dummy;
  while(cur != NULL){
    if(new_Node_Prior->weight <= cur->weight){
       break;
    }
    else{
      prev = cur;
      cur = cur->next;
    }
  }
  prev->next = new_Node_Prior;
  new_Node_Prior->next = cur;
  priority_Head = dummy.next;
}


node *compress::dequeuePrior(){
  node *returnNode = priority_Head;
  if(priority_Head == NULL)
    return NULL;

  priority_Head = priority_Head->next;
  returnNode->next = NULL;
  return returnNode;  

}

node *compress::combineTwoNodes(node *firstNode,node *secondNode){
  node *newNode = new node;
  newNode->info = 't';
  if(newNode == NULL){
    cerr << "Couldn't allocte space for new node";
    return NULL;
  }
  newNode->weight = firstNode->weight + secondNode->weight;
  newNode->left = firstNode;
  newNode->right = secondNode;
  return newNode;
}

void compress::constructTree(){
  node *interNode;
  while(priority_Head->next != NULL){
    interNode = dequeuePrior();
    interNode = combineTwoNodes(interNode,dequeuePrior());
    enqueueTree(interNode);
  }
}
 
void compress::preTravTree(node *head){
  if(head->left == NULL){
    cout << head->info << " "; 
    return;
  }
  preTravTree(head->left);
  preTravTree(head->right);
}

void compress::saveTree(node *head){
  if(head == NULL){
    cerr<< " You screwed up " << endl;
    return;
  }
  if(head->left == NULL && head->right == NULL){
    //if we reach a leaf node I need to print a bit with 1
    bit_to_be_written = 1 << ( CHAR_SIZE - num_written_char - 1);
    num_written_char++;
    //move tha character to the current stream of bit to be written
    char_to_be_written = char_to_be_written | bit_to_be_written;
    if( num_written_char == 8){
      fileToOutput << (unsigned char)char_to_be_written;
      char_to_be_written = 0x00;
      num_written_char = 0;  
    }
    unsigned char char_from_tree = (unsigned char) head->info;
    unsigned char mask = 1 << (CHAR_SIZE - 1);
    for(short i = 0;  i < CHAR_SIZE; i++){
      short result = char_from_tree & mask;
      if(result == 0){
        num_written_char++;
        if( num_written_char == 8){
          fileToOutput << (unsigned char)char_to_be_written;
          char_to_be_written = 0x00;
          num_written_char = 0;  
        }       
      }else{
        bit_to_be_written = 1 << ( CHAR_SIZE - num_written_char - 1);
        num_written_char++;
        char_to_be_written = char_to_be_written | bit_to_be_written;
        if( num_written_char == 8){
          fileToOutput << (unsigned char)char_to_be_written;
          char_to_be_written = 0x00;
          num_written_char = 0;  
        }
      }
     mask = mask >> 1;
    } 
    return;
  }else if(head->left == NULL || head->right == NULL){
    cerr << "Something unexpected happened";
    return;
  }
  saveTree(head->left);
  saveTree(head->right);
  num_written_char++;
  if( num_written_char == 8){
    fileToOutput << (unsigned char)char_to_be_written;
    char_to_be_written = 0x00;
    num_written_char = 0;  
  }
}

void compress::createTable(node *head){
  if(head->left == NULL && head->right == NULL){
    cout << "Char :" << head->info << (unsigned int) head->info << " encoded ";
    weight2[(int)head->info].char_to_be_w = int_to_be_written;
    weight2[(int)head->info].bits = num_written_char;
    unsigned long int char_from_tree = (unsigned long int) weight2[(int)head->info].char_to_be_w;
    unsigned long int mask = 0;
    mask = 1 << (CHAR_SIZE*4  - 1);
    mask = mask << 32;
    for(short i = 0;  i < CHAR_SIZE*8; i++){
    unsigned long int result = char_from_tree & mask;

     if(result == 0){
    cout << "0";
    }else{
    cout << "1";
    }
    mask = mask >> 1;
    } 
    cout<< " Bits in seq: " << num_written_char << "  " << head->info;
    cout << endl;
  return;
  }
  num_written_char++;
  int_to_be_written = int_to_be_written << 1;
  createTable(head->left);
  unsigned long int bit_input = 1 << (0);
  int_to_be_written = int_to_be_written | bit_input;
  createTable(head->right);
  int_to_be_written = int_to_be_written >> 1;
  num_written_char--;
}



void compress::emptyW(){
  //empties if there is anything unprinted
  if(num_written_char != 0){
    cout<<"Check OUT";
    fileToOutput << char_to_be_written;

  }
  char_to_be_written = 0x00;
  num_written_char = 0;
}

void compress::writeFile(){
  num_written_char = 0;
  char readChar;
  file.clear();
  file.seekg(0,file.beg);
  while(file.get(readChar)){
    cout << "To output2: " << readChar << " bit encoding :" ;
    unsigned long int char_from_tree = weight2[(int)readChar].char_to_be_w;
    unsigned long int mask = 0;
    mask = 1 << (weight2[(int)readChar].bits - 1);
    for(short i = weight2[(int)readChar].bits;  i > 0; i--){
      unsigned long int result = char_from_tree & mask;
      if(result == 0){
        num_written_char++;
        cout << "0";
        if( num_written_char == 8){
          fileToOutput << (unsigned char)char_to_be_written;
          char_to_be_written = 0x00;
          num_written_char = 0;  
        }       
      }else{
        cout << "1";
        bit_to_be_written = 1 << (CHAR_SIZE - num_written_char - 1);
        num_written_char++;
        char_to_be_written = char_to_be_written | bit_to_be_written;
        if( num_written_char == 8){
          fileToOutput << (unsigned char)char_to_be_written;
          char_to_be_written = 0x00;
          num_written_char = 0;  
        }
      }
     mask = mask >> 1;
    } 
    cout << endl;
  }
  cout << endl;

}

void compress::emptyWEOF(){
    unsigned long int char_from_tree = weight2[200].char_to_be_w;
    unsigned long int mask = 0;
    mask = 1 << (weight2[200].bits - 1);
    cout << "BITS " << weight2[200].bits;
    cout << " " << char_from_tree << endl;
    for(short i = weight2[200].bits;  i > 0; i--){
     
      int result = char_from_tree & mask;
      if(result == 0){
        num_written_char++;
        cout << "0";
        if( num_written_char == 8){
          fileToOutput << (unsigned char)char_to_be_written;
          char_to_be_written = 0x00;
          num_written_char = 0;  
        }       
      }else{
        cout << "1";
        bit_to_be_written = 1 << (CHAR_SIZE - num_written_char - 1);
        num_written_char++;
        char_to_be_written = char_to_be_written | bit_to_be_written;
        if( num_written_char == 8){
          fileToOutput << (unsigned char)char_to_be_written;
          char_to_be_written = 0x00;
          num_written_char = 0;  
        }
      }
     mask = mask >> 1;
    } 
      cout << "CHARS Written: " << num_written_char << " ";
      cout << endl;
}
