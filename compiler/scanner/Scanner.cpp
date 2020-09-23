#include <cstddef>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<iostream>



char* re2post(char *re){
	int nalt, natom;
	static char buf[8000];
	char *dst;
	struct {
		int nalt;
		int natom;
	} paren[100], *p;
	
	p = paren;
	dst = buf;
	nalt = 0;
	natom = 0;
	if(strlen(re) >= sizeof buf/2)
		return NULL;
	for(; *re; re++){
		switch(*re){
		case '(':
			if(natom > 1){
				--natom;
				*dst++ = '.';
			}
			if(p >= paren+100)
				return NULL;
			p->nalt = nalt;
			p->natom = natom;
			p++;
			nalt = 0;
			natom = 0;
			break;
		case '|':
			if(natom == 0)
				return NULL;
			while(--natom > 0)
				*dst++ = '.';
			nalt++;
			break;
		case ')':
			if(p == paren)
				return NULL;
			if(natom == 0)
				return NULL;
			while(--natom > 0)
				*dst++ = '.';
			for(; nalt > 0; nalt--)
				*dst++ = '|';
			--p;
			nalt = p->nalt;
			natom = p->natom;
			natom++;
			break;
		case '*':
		case '+':
		case '?':
			if(natom == 0)
				return NULL;
			*dst++ = *re;
			break;
		default:
			if(natom > 1){
				--natom;
				*dst++ = '.';
			}
			*dst++ = *re;
			natom++;
			break;
		}
	}
	if(p != paren)
		return NULL;
	while(--natom > 0)
		*dst++ = '.';
	for(; nalt > 0; nalt--)
		*dst++ = '|';
	*dst = 0;
	return buf;
}



using namespace std; 

int nstate;

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
    nstate++;
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
	Match = 257,
	Split = 256
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
            // cout<<"case ."<<"\n";
			break;
		case '|':	/* alternate */
			e2 = my_stack.top();
            my_stack.pop();
			e1 = my_stack.top();
            my_stack.pop();
			s = new State(Split, e1.start, e2.start);
			my_stack.push(Fragment(s, append(e1.out, e2.out)));
            // cout<<"case |"<<"\n";

			break;
		case '?':	/* zero or one */
			e = my_stack.top();
            my_stack.pop();
			s = new State(Split, e.start, NULL);
			my_stack.push(Fragment(s, append(e.out, list1(&s->out1))));
            // cout<<"case ?"<<"\n";
			break;
		case '*':	/* zero or more */
			e = my_stack.top();
            my_stack.pop();
			s = new State(Split, e.start, NULL);
			patch(e.out, s);
			my_stack.push(Fragment(s, list1(&s->out1)));
            // cout<<"case *"<<"\n";
			break;
		case '+':	/* one or more */
			e = my_stack.top();
            my_stack.pop();
			s = new State(Split, e.start, NULL);
			patch(e.out, s);
			my_stack.push(Fragment(s, list1(&s->out1)));
            // cout<<"case +"<<"\n";
			break;
		}
	}
    // cout<<"out of switch"<<"\n";;
    // cout<<"stacksize:"<<my_stack.size()<<"\n";
	e = my_stack.top();
    // cout<<"e.start.c:"<<e.start->c<<"\n";
    my_stack.pop();
    // cout<<"stacksize after pop:"<<my_stack.size()<<"\n";
	// if(my_stack.size() != 1)
	// 	return NULL;    
    patch(e.out, &matchstate);
    // cout<<"new e.out.s.c:"<<e.out->s->c<<"\n";
    // cout<<"new e.start.c:"<<e.start->c<<"\n";
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

void
step(List *clist, int c, List *nlist)
{
	int i;
	State *s;

	listid++;
	nlist->n = 0;
	for(i=0; i<clist->n; i++){
		s = clist->s[i];
		if(s->c == c)
			addstate(nlist, s->out);
	}
}

/* Run NFA to determine whether it matches s. */
int
match(State *start, char *s)
{
	int i, c;
	List *clist, *nlist, *t;


	clist = startlist(start, &l1);
	nlist = &l2;
	for(; *s; s++){
		c = *s & 0xFF;
		step(clist, c, nlist);
		t = clist; clist = nlist; nlist = t;	/* swap clist, nlist */
	}
	return ismatch(clist);
}

int
main(int argc, char **argv)
{
	int i;
	char *post;
	State *start;

	if(argc < 3){
		fprintf(stderr, "usage: nfa regexp string...\n");
		return 1;
	}
	
	post = re2post(argv[1]);
	if(post == NULL){
		fprintf(stderr, "bad regexp %s\n", argv[1]);
		return 1;
	}

	start = post2nfa(post);
	if(start == NULL){
		fprintf(stderr, "error in post2nfa %s\n", post);
		return 1;
	}

    l1.s = (State**) malloc(nstate*sizeof l1.s[0]);
	l2.s = (State**) malloc(nstate*sizeof l2.s[0]);
    
	for(i=2; i<argc; i++)
		if(match(start, argv[i]))
			printf("%s\n", argv[i]);
	return 0;
}