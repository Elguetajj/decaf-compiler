#include "State.hpp"

State::State(int c, State *out, State *out1){
    this->lastlist = 0;
    this->c = c;
	this->out = out;
	this->out1 = out1;
}

State::State(int c){
    this->c = c;
}