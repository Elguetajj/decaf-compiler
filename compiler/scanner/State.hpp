class State
{
private:
    int c;
	State *out;
	State *out1;
	int lastlist;

public:
    State(int c, State *out, State *out1);
    State(int c);
  
};
