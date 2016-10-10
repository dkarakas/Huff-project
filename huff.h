#ifndef __HUFF_H_INCLUDED_
#define __HUFF_H_INCLUDED_


#include <iostream>
#include <fstream>


class compress{
  public:
   bool fileOpen(char *fileName);
   void weights();
   void tWeights();
   compress();
   ~compress();

  private:


  protected:
   std::fstream file;
   int weight[256];
};

class node{
  public:
    node *left;
    node *right;
    node *next;
    char info; 
    long int weight;
};

#endif
