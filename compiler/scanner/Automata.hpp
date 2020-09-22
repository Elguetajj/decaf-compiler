#include <vector>

#define NO_OF_CHARS 256

class Automata
{
private:

public:
    void buildTransitionTable(char *pattern, int pattern_length, std::vector<std::vector<int> > ptransition_table);
    void printtable(std::vector<std::vector<int> > ptransition_table);
    void patternsearch(char pattern[], char text[]);
};