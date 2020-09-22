#include<iostream>
#include<cstring>
#include "Automata.hpp" // Bring the ClassOne declaration into "view" of the compiler
using namespace std;


int main(int argc, char* argv[])
{
  char x[] = "ABC";
  char y[] = "ABBCxyzABB";
  Automata auttt;
  auttt.patternsearch(x,y);
  return 0;
}