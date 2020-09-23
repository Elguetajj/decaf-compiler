#include <cstddef>
#include <stack>          // std::stack

using namespace std; 
  
class State
{
private:
    
public:
    int c;
	State *out;
	State *out1;
	int lastlist;
    State(int c, State *out, State *out1);
    State(int c);
};

State::State(int c, State *out, State *out1)
{
    this->c = c;
    this->out = out;
    this->out1 = out1;
    this->lastlist = 0;
}

State::State(int c)
{
    this->c = c;
}



union Ptrlist
{
	Ptrlist *next;
	State *s;
};

Ptrlist* list1(State **outp)
{
	Ptrlist *l;
	
	l = (Ptrlist*)outp;
	l->next = NULL;
	return l;
}

Ptrlist* append(Ptrlist *l1, Ptrlist *l2)
{
	Ptrlist *oldl1;
	
	oldl1 = l1;
	while(l1->next)
		l1 = l1->next;
	l1->next = l2;
	return oldl1;
}

/* Patch the list of states at out to point to start. */
void patch(Ptrlist *l, State *s)
{
	Ptrlist *next;
	
	for(; l; l=next){
		next = l->next;
		l->s = s;
	}
}



class Fragment
{
private:
    
public:
    State *start;
	Ptrlist *out;
    Fragment(State *start, Ptrlist *out);
    Fragment() = default;
};

Fragment::Fragment(State *start, Ptrlist *out)
{
    this->start = start;
    this->out = out;
}


// typedef struct Frag Frag;

// struct Fragment
// {
// 	State *start;
// 	Ptrlist *out;
// };

// /* Initialize Frag struct. */
// Fragment
// Frag(State *start, Ptrlist *out)
// {
// 	Frag n = { start, out };
// 	return n;
// }


enum
{
	Match = 256,
	Split = 257
};
State matchstate =  State(Match);


State* post2nfa(char postfix[])
{
	char *p;
	Fragment e1, e2, e;
    std::stack <Fragment> my_stack;
	State *s;
	// fprintf(stderr, "postfix: %s\n", postfix);

	if(postfix == NULL)
		return NULL;

	for(p=postfix; *p; p++){
		switch(*p){
		default:
			s = new State(*p, NULL, NULL);
			my_stack.push(Fragment(s, list1(&s->out)));
			break;
		case '.':	/* catenate */
			e2 = my_stack.top();
            my_stack.pop();
			e1 = my_stack.top();
            my_stack.pop();
			patch(e1.out, e2.start);
			my_stack.push(Fragment(e1.start, e2.out));
			break;
		case '|':	/* alternate */
			e2 = my_stack.top();
            my_stack.pop();
			e1 = my_stack.top();
            my_stack.pop();
			s = new State(Split, e1.start, e2.start);
			my_stack.push(Fragment(s, append(e1.out, e2.out)));
			break;
		case '?':	/* zero or one */
			e = my_stack.top();
            my_stack.pop();
			s = new State(Split, e.start, NULL);
			my_stack.push(Fragment(s, append(e.out, list1(&s->out1))));
			break;
		case '*':	/* zero or more */
			e = my_stack.top();
            my_stack.pop();
			s = new State(Split, e.start, NULL);
			patch(e.out, s);
			my_stack.push(Fragment(s, list1(&s->out1)));
			break;
		case '+':	/* one or more */
			e = my_stack.top();
            my_stack.pop();
			s = new State(Split, e.start, NULL);
			patch(e.out, s);
			my_stack.push(Fragment(s, list1(&s->out1)));
			break;
		}
	}

	e = my_stack.top();
    my_stack.pop();
	if(my_stack.size() != 1)
		return NULL;

	patch(e.out, &matchstate);
	return e.start;
}

// typedef struct List List;
// struct List
// {
// 	State **s;
// 	int n;
// };

class List
{
private:
    /* data */
public:
    State **s;
	int n;
};


List l1, l2;
static int listid;

void addstate(List*, State*);
void step(List*, int, List*);

/* Compute initial state list */
List*
startlist(State *start, List *l)
{
	l->n = 0;
	listid++;
	addstate(l, start);
	return l;
}

/* Check whether state list contains a match. */
int
ismatch(List *l)
{
	int i;

	for(i=0; i<l->n; i++)
		if(l->s[i] == &matchstate)
			return 1;
	return 0;
}

/* Add s to l, following unlabeled arrows. */
void
addstate(List *l, State *s)
{
	if(s == NULL || s->lastlist == listid)
		return;
	s->lastlist = listid;
	if(s->c == Split){
		/* follow unlabeled arrows */
		addstate(l, s->out);
		addstate(l, s->out1);
		return;
	}
	l->s[l->n++] = s;
}















class Scanner
{
private:
    /* data */
public:
    Scanner(/* args */);
    ~Scanner();
};

Scanner::Scanner(/* args */)
{
}

Scanner::~Scanner()
{
}
