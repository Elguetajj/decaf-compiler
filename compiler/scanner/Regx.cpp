#include <cstddef>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<iostream>

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

enum MatchSplit
{
	Match = 257,
	Split = 256
};


class List
{
private:
    /* data */
public:
    State **s;
	int n;
};

class Regx
{
private:
    State matchstate=  State(Match);;
    List l1, l2;
    int listid = 0;
    int nstate;
public:
    char* re2post(char *re);
    State* post2nfa(char postfix[]);
    void addstate(List*, State*);
    void step(List*, int, List*);
    List* startlist(State *start, List *l);
    int ismatch(List *l);
    int match(State *start, char *s);
    bool eval(string reg,string str2match);
	bool eval(State *start, string str2match);
    State* newState(int c, State *out, State *out1);
	State* compile(string str2compile);

};

State* Regx::newState(int c, State *out, State *out1){
    State* s = new State(c,out,out1);
    nstate++;
    return s;
}

char* Regx::re2post(char *re){
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
		// cout<<"re:"<<*re<<"\n";
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
		case '/':
			if(natom > 1){
				--natom;
				*dst++ = '.';
			}
			*dst++ = *re;
			re++;
			*dst++ = *re;
			natom++;

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
		// cout<<"buff:"<<buf<<"\n";
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



State* Regx::post2nfa(char postfix[])
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
			s = newState(*p, NULL, NULL);
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
			s = newState(Split, e1.start, e2.start);
			my_stack.push(Fragment(s, append(e1.out, e2.out)));
            // cout<<"case |"<<"\n";

			break;
		case '?':	/* zero or one */
			e = my_stack.top();
            my_stack.pop();
			s = newState(Split, e.start, NULL);
			my_stack.push(Fragment(s, append(e.out, list1(&s->out1))));
            // cout<<"case ?"<<"\n";
			break;
		case '*':	/* zero or more */
			e = my_stack.top();
            my_stack.pop();
			s = newState(Split, e.start, NULL);
			patch(e.out, s);
			my_stack.push(Fragment(s, list1(&s->out1)));
            // cout<<"case *"<<"\n";
			break;
		case '+':	/* one or more */
			e = my_stack.top();
            my_stack.pop();
			s = newState(Split, e.start, NULL);
			patch(e.out, s);
			my_stack.push(Fragment(s, list1(&s->out1)));
            // cout<<"case +"<<"\n";
			break;
		case '/':
			// cout<<"p:"<<*p<<'\n';
			p++;
			// cout<<"pafter:"<<*p<<'\n';
			s = newState(*p, NULL, NULL);
			my_stack.push(Fragment(s, list1(&s->out)));
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

/* Compute initial state list */
List* Regx::startlist(State *start, List *l)
{
	l->n = 0;
	this->listid++;
	addstate(l, start);
	return l;
}

/* Check whether state list contains a match. */
int Regx::ismatch(List *l)
{
	int i;

	for(i=0; i<l->n; i++)
		if(l->s[i] == &matchstate)
			return 1;
	return 0;
}

/* Add s to l, following unlabeled arrows. */
void Regx::addstate(List *l, State *s)
{
	if(s == NULL || s->lastlist == this->listid)
		return;
	s->lastlist = this->listid;
	if(s->c == Split){
		/* follow unlabeled arrows */
		addstate(l, s->out);
		addstate(l, s->out1);
		return;
	}
	l->s[l->n++] = s;
}

void Regx::step(List *clist, int c, List *nlist)
{
	int i;
	State *s;

	this->listid++;
	nlist->n = 0;
	for(i=0; i<clist->n; i++){
		s = clist->s[i];
		if(s->c == c)
			addstate(nlist, s->out);
	}
}

/* Run NFA to determine whether it matches s. */
int Regx::match(State *start, char *s)
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

bool Regx::eval(string reg,string str2match)
{
	int i;
	char *post;
	State *start;
	char * regex = &reg[0];
	char* string2match = &str2match[0];

	post = re2post(regex);
	if(post == NULL){
		fprintf(stderr, "bad regexp %s\n", regex);
		return 1;
	}

	start = post2nfa(post);
	if(start == NULL){
		fprintf(stderr, "error in post2nfa %s\n", post);
		return 1;
	}

    l1.s = (State**) malloc(nstate*sizeof l1.s[0]);
	l2.s = (State**) malloc(nstate*sizeof l2.s[0]);
    
    if(match(start, string2match))
        return true;
    else
        return false;
    
}

bool Regx::eval(State *start,string str2match)
{
	char* string2match = &str2match[0];

    l1.s = (State**) malloc(nstate*sizeof l1.s[0]);
	l2.s = (State**) malloc(nstate*sizeof l2.s[0]);
    
    if(match(start, string2match))
        return true;
    else
        return false;  
}


State *Regx::compile(string reg)
{
	int i;
	char *post;
	State *start;
	char * regex = &reg[0];

	post = re2post(regex);
	if(post == NULL){
		fprintf(stderr, "bad regexp %s\n", regex);
		return NULL;
	}

	start = post2nfa(post);
	if(start == NULL){
		fprintf(stderr, "error in post2nfa %s\n", post);
		return NULL;
	}
	
	return start;
}







