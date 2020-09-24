#include"Regx.cpp"
#include"Token.cpp"
#include"Error.cpp"
#include<fstream>
#include<sstream>
#include<iostream>

class Scanner
{
private:
    Regx re;

public:
    void tokenize(char filename[]);
    Token tokenstream;
    Error errorstream;
};



void Scanner::tokenize(char filename[])
{
    std::string mytext;
    fstream MyReadFile(filename);
    if(MyReadFile) {
        ostringstream ss;
        ss << MyReadFile.rdbuf();
        mytext = ss.str();
    }
    MyReadFile.close();

    //loop
    char *str = &mytext[0];
    char *p;
    std::string lexeme;
    std::string errlex = "";
    char *lex = &lexeme[0];
    std::string tokenValue = "";
    int line=0;
    int position=1;

    int errline=0;
    int errposition=0;
    std::string charac;

    char reg0[] = "h";
    char reg[] = "(ho)";
    char reg2[] = "(hol)";
    


    for(p=str; *p; p++){
        // cout<<"p:"<<p<<"\n";
        // cout<<"current letter: "<<*p<<"\n";
        lexeme += *p;
        charac = *p;
        // cout<<"lexeme:"<<"\n"<<lexeme<<"\n";
       
        if (charac == "\n")
        {
            position=0;
            line++;
        }
        
        if (re.eval(reg0,lex))
        {
            if(errlex.size()>0){
                errorstream = Error(errlex,errposition,errline);
                errorstream.printErrors();
                errlex = "";

            }
            // cout<<"possible token T_H found:"<<lexeme<<"\n";
            if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
            }   
        }
        else if(re.eval(reg,lex))
        {
            if(errlex.size()>0){
                errorstream = Error(errlex,errposition,errline);
                errorstream.printErrors();
                errlex = "";
            }
            // cout<<"possible token T_HO found:"<<lexeme<<"\n";
            if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
            }            
        }
        else if (re.eval(reg2,lex))
        {
            if(errlex.size()>0){
                errorstream = Error(errlex,errposition,errline);
                errorstream.printErrors();
                errlex = "";

            }
            // cout<<"possible token T_HOL found:"<<lexeme<<"\n";
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
            } 
        }
        else
        {
            if(tokenValue.size()>0)
            {
                tokenstream = Token(tokenValue,tokenValue,position-tokenValue.size(),line);
                tokenstream.printTokens();
                tokenValue = "";
                lexeme = "";
                position--;
                p--;
            }
            else
            {
                if (strlen(p)==1)
                {
                    errlex += lexeme;
                    errorstream = Error(errlex,errposition,errline);
                    errorstream.printErrors();
                }
                else
                {
                    errline = line;
                    errposition = position;
                    errlex += lexeme;
                    lexeme ="";  
                } 
            }
        }
        position++;

    }
}

