#include"Error.cpp"

class ErrorStream
{
private:
    /* data */
public:
    Error* head;
    Error* tail;
    

    ErrorStream();
    void addError(Error* Error);
    void printErrorStream();
};

ErrorStream::ErrorStream()
{
    this->head = NULL;
    this->tail=NULL;
}

void ErrorStream::addError(Error* error)
{
    if(this->head == NULL)
    {
        this->head = error;
        this->tail = error;
    }else
    {
        tail->next = error;
        tail = tail->next;
    }
}

void ErrorStream::printErrorStream()
{
    Error* current = this->head;
    while(current != tail->next)
    {
        cout<<"<<"<<"Error message:"<<": lexeme '"<<current->value<<"' could not be tokenized,"<<"line:"<<current->line<<", position:"<<current->position<<">>"<<"\n";
        current = current->next;
    }
}


