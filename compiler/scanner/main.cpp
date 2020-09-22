#include<iostream>
#include<cstring>
#include "Automata.hpp" // Bring the ClassOne declaration into "view" of the compiler
#include "Regex.hpp"
using namespace std;


int main(int argc, char* argv[])
{
  char x[] = "ABC";
  char y[] = "ABBCxyzABB";
  char re[] = "a(bb)+a";
  Regex reng;
  char* post = reng.re2post(re);
  cout<<post<<'\n';

  // Automata auttt;
  // auttt.patternsearch(x,y);
  return 0;
}