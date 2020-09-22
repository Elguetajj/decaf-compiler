#include <stdio.h>
#include <string.h>
#include<iostream>
#include<cstring>
#include <vector>
#include "Automata.hpp"

using namespace std; 

void Automata::buildTransitionTable(char *pattern, int pattern_length,std::vector<std::vector<int> > ptransition_table){
   
   int i, longestprefixsuffix = 0, y;
   // put entries in first row
   ptransition_table[0][pattern[0]] = 1;
   //  for (y =0; y < NO_OF_CHARS; y++)
   //  TT[0][y] = 0;
   //  TT[0][pattern[0]] = 1;
   // put entries in other rows

   
   for (i = 1; i<= pattern_length; i++) { // Copy values from row at index longestprefixsuffix
      for (y = 0; y < NO_OF_CHARS; y++)
         ptransition_table[i][y] = ptransition_table[longestprefixsuffix][y];
      // Update the entry
      ptransition_table[i][pattern[i]] = i + 1;
      // Update lps for next row to be filled
      if (i < pattern_length)
         longestprefixsuffix = ptransition_table[longestprefixsuffix][pattern[i]]; // TT is the 2D array which is being constructed.
   }

   printtable(ptransition_table);
   cout<<"\n";
}

void Automata::patternsearch(char pattern[], char text[]){
   int P = strlen(pattern);
   int T = strlen(text);
   std::vector<std::vector<int> > transition_table;
   transition_table.resize(P+1, std::vector<int>(NO_OF_CHARS, 0));
   buildTransitionTable(pattern, P, transition_table);
   // process text over FA.
   printtable(transition_table);
   int i, j=0,ascii;
   for (i = 0; i < T; i++) {
      cout<<"\n"<<i;
      ascii = text[i];
      cout<<"\n"<<text[i]<<":"<< ascii;
      j = transition_table[j][text[i]];
      cout<<"\n"<< j;
      cout<<"\n"<< P;
      if (j == P) {
         cout<<"\n pattern is found at index: "<< i-P+1;
      }
   }
}

void Automata::printtable (std::vector<std::vector<int> > ptransition_table){

   for(int i = 0; i< ptransition_table.size();i++){
      for(int j = 0; j < ptransition_table[0].size(); j++){
         cout<<ptransition_table[i][j];
      }
      cout<<"\n";
   }

}






