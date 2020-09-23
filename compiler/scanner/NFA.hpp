#include"State.hpp"

enum
{
	Match = 256,
	Split = 257
};

class NFA{
    private:
        State matchstate;
        int nstate;

    public:
        NFA();
        State* post2nfa(char *postfix);
};