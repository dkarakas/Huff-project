#ifndef __HUFF_H_INCLUDED_
#define __HUFF_H_INCLUDED_


#include <iostream>
#include <fstream>

class node{
  public:
    //node(char,long int){};
    
    //properties
    node *left;
    node *right;
    node *next;
    char info; 
    long int weight;
};

class table{
 public:
   long int char_to_be_w;
   short bits;
};
class compress{
  public:
   //huff function
   bool fileOpen(char *fileName);
   void weights();
   void tWeights();
   compress();
   ~compress();
   node *createNode(char,long int);
   //Priority queueu func
   void createPriorityQueue();
   void enqueuePrior(long int, char);
   void enqueueTree(node *);
   node *dequeuePrior();
   node *combineTwoNodes(node *, node*);
   void printQueue();
   void constructTree();
   void preTravTree(node *);
   void saveTree(node*);
   node *getHead(){return priority_Head;};
   void emptyW();
   void createTable(node *);
   void writeFile();
   void emptyWEOF();
   //unhuff function
   void read_file();
   node *stack_pop();
   void stack_push(node *);
   node *getHeadStack(){return stack_aux;};
   void decomp();


  private:
   //huff function
   node *priority_Head; //is the head of the huff tree 
   std::fstream file; //will contain the file to be the outputed
   unsigned long int save_temp_bits;//creating a table
   std::fstream fileToOutput;
	 long int weight[256]; //will contain how many time I see char in the first read
   table weight2[256]; //will contain how many time I see char in the first read
   unsigned char char_to_be_written; //will contain a stream of bit to be written
   short num_written_char; // number of bits that are written in char_to_be_written
   unsigned char bit_to_be_written; // the exact number to be written
   //unhuff function
   unsigned char read_char;
   short num_bit_read;
   node *stack_aux;
};


#endif
