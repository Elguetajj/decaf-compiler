#include<iostream>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std; 


class Error
{
private:
    
public:
    std::string value;
    int position;
    int line;

    Error* next;
    Error(std::string value, int position, int line);
    Error() = default;
    void setNext(std::string value, int position, int line);
    void printErrors();

};

Error::Error(std::string value, int position, int line)
{
    
    this->value = value;
    this->position = position;
    this->line = line;
}

void Error::setNext(std::string value, int position, int line)
{
    this->next = new Error(value,position,line);
}

void Error::printErrors()
{
    cout<<"<<"<<"Error message:"<<": lexeme '"<<this->value<<"' could not be tokenized,"<<"line:"<<this->line<<", position:"<<this->position<<">>"<<"\n";
}




