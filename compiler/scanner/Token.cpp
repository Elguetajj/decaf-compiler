#include<iostream>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std; 


class Token
{
private:
   
    

public:
    std::string token_type;
    std::string value;
    int position;
    int line;

    Token* next;
    Token(std::string token_type, std::string value, int position, int line);
    Token() = default;
    void setNext(std::string token_type, std::string value, int position, int line);
    void printTokens();

};

Token::Token(std::string token_type, std::string value, int position, int line)
{
    this->token_type = token_type;
    this->value = value;
    this->position = position;
    this->line = line;
}

void Token::setNext(std::string token_type, std::string value, int position, int line)
{
    this->next = new Token(token_type, value, position, line);
}

void Token::printTokens()
{
}




