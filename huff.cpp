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
  comp.createPriorityQueue();
  comp.printQueue();
  cout << endl;
  comp.constructTree();
  comp.preTravTree(comp.getHead());
  cout << endl;
  //comp.tWeights();
  return 0;
}






//COMPRESS CLASSS

compress::compress(){
     
     //initilize the weight array
     for(int i = 0; i < 256; i++){weight[i] = 0;}
    
     priority_Head = NULL;
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





