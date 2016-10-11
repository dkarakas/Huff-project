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


class compress{
  public:
   bool fileOpen(char *fileName);
   void weights();
   void tWeights();
   compress();
   ~compress();
   node *createNode(char,long int);
   //Priority queueu func
   void createPriorityQueue();
   void enqueuePrior(long int, char);
   void printQueue();


  private:
   node *priority_Head;
   std::fstream file;
   long int weight[256];
   int weight_total;
};


#endif
