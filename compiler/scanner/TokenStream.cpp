#include"Token.cpp"

class TokenStream
{
private:
    /* data */
public:
    Token* head;
    Token* tail;
    

    TokenStream();
    void addToken(Token* token);
    void printTokenStream();
};

TokenStream::TokenStream()
{
    this->head = NULL;
    this->tail=NULL;
}

void TokenStream::addToken(Token* token)
{
    if(this->head == NULL)
    {
        this->head = token;
        this->tail = token;
    }else
    {
        tail->next = token;
        tail = tail->next;
    }
}

void TokenStream::printTokenStream()
{
    Token* current = this->head;
    while(current->next != NULL)
    {
        current->printTokens();
        current = current->next;
    }
}


