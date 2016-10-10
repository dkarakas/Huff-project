#ifndef __HUFF_H_INCLUDED_
#define __HUFF_H_INCLUDED_


#include <iostream>
#include <fstream>


class compress{
  public:
   void weights();
   void tWeights();
   compress(){weight[256] = {0};};


  private:


  protected:
    int weight[256];
    std::fstream file;
   

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
