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
    while(current->next != NULL)
    {
        current->printErrors();
        current = current->next;
    }
}


