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
  comp.read_file();
  comp.preTravTree(comp.getHeadStack());
  comp.decomp();

  cend = (double) clock();

  cout << ((double) cend - (double) cstart)/CLOCKS_PER_SEC<<endl;
  
  return 0;
}


//COMPRESS CLASSS

compress::compress(){
     
   //initilize the weight array
   for(int i = 0; i < 256; i++){weight[i] = 0;}
    
   priority_Head = NULL;
   stack_aux = NULL;
   read_char = 0x00;
   num_bit_read = 0;
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
  std::string unhuff(".unhuff");
  //opens the file and if it is false the program gives an error
  file.open(fileName, fstream::in | fstream:: out | fstream::app);
  if(!file.is_open()){
    return 0;
  }

  file_s = file_s + unhuff;
  fileToOutput.open(file_s.c_str(), fstream::out);
  if(!fileToOutput.is_open()){
    return 0;
  }
  return 1;
}

void compress::read_file(){
  file >> noskipws >>  read_char;
  bool not_done = true;
  unsigned char mask = 1 << (CHAR_SIZE - num_bit_read - 1);
  while(not_done){
   int result = read_char & mask;
   num_bit_read++;
   if(result == 0){
     node * firstNode = stack_pop();
     node * secondNode = stack_pop();
     if(secondNode == NULL){
       stack_push(firstNode);
       not_done = false;
       break;
     }
     if(num_bit_read >= 8){
       num_bit_read = 0;
       file >> noskipws >>read_char;
     }

     node * new_node = combineTwoNodes(secondNode, firstNode);
     stack_push(new_node);
   }else{
     unsigned char to_be_pushed = 0x00;

     if(num_bit_read >= 8){
       file >> noskipws >> read_char;
       num_bit_read = 0;
       to_be_pushed = read_char;
       file >> noskipws >> read_char;
     }else{
       to_be_pushed = read_char << num_bit_read;
       file >> noskipws >> read_char;
       for(int i = 0; i < num_bit_read; i++){
         mask = 1 << (CHAR_SIZE - i - 1);
         result = read_char & mask; 
         if(result != 0){
            mask = 1 << (num_bit_read - i - 1);
            to_be_pushed = to_be_pushed | mask;
         }
       }
     }
     node * new_node = createNode(to_be_pushed,0);
     stack_push(new_node); 
   }
   mask = 1 << (CHAR_SIZE - num_bit_read - 1);
  }

}

void compress::decomp(){
   node *cur_loc_tree = stack_aux;
   char aux;
   bool end = false;
   while(file.get(aux)&&!end){
     unsigned char mask = 1 << (CHAR_SIZE - 1);
     for(short i = 0; i < CHAR_SIZE; i++){
       short result = aux & mask;
       if(result == 0){
          cur_loc_tree = cur_loc_tree->left;
          if(cur_loc_tree->left == NULL || cur_loc_tree->right == NULL){
            if(cur_loc_tree->info != 200){
              fileToOutput << cur_loc_tree->info;
            }else{/*cout << "HIT"*/; end = true;break;}
            cur_loc_tree = stack_aux;
          }
       }else{
          cur_loc_tree = cur_loc_tree->right;
          if(cur_loc_tree->left == NULL || cur_loc_tree->right == NULL){
            if(cur_loc_tree->info != 200){
              fileToOutput << cur_loc_tree->info;
            }else{/*cout << "HIT"*/; end = true;break;}
            cur_loc_tree = stack_aux;
          }
       }
       mask = mask >> 1;
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


node * compress::stack_pop(){
  node *return_node;
  if(stack_aux == NULL){
    return NULL;
  }
  return_node = stack_aux; 
  stack_aux = stack_aux->next;
  return return_node;
}

void compress::stack_push(node * newNode){
  if(newNode == NULL){
    cerr << "Invalid try to push\n";
    return;
  }
  newNode->next = stack_aux;
  stack_aux = newNode;
}

void compress::preTravTree(node *head){
  if(head->left == NULL && head->right == NULL){
    //cout << head->info << " "; 
    return;
  }
  preTravTree(head->left);
  preTravTree(head->right);
}

