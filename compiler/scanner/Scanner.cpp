#include"Regx.cpp"
#include"TokenStream.cpp"
#include"ErrorStream.cpp"
#include<fstream>
#include<sstream>
#include<iostream>
#include <map>


class Scanner
{
private:
    Regx re;

public:
    void tokenize(char filename[]);
    TokenStream tokenstream;
    ErrorStream errorstream;
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
        lexeme += *p;
        charac = *p;

        if (charac == "\n")
        {
            position=0;
            line++;
        }
        
        if (re.eval(reg0,lex))
        {
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
            if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
            }   
        }
        else if(re.eval(reg,lex))
        {
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
            if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
            }            
        }
        else if (re.eval(reg2,lex))
        {
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";

            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
            } 
        }
        else
        {
            if(tokenValue.size()>0)
            {
                tokenstream.addToken(new Token("T_"+tokenValue,tokenValue,position-tokenValue.size(),line)); 
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
                    errorstream.addError(new Error(errlex,errposition,errline));
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
    cout<<"Token Stream:"<<"\n";
    tokenstream.printTokenStream();
    cout<<"\n"<<"Error Stream:"<<"\n";
    errorstream.printErrorStream();
}

std::map<string, string> basics =
{
    {"letter" , "(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_)"},
    {"digit", "(0|1|2|3|4|5|6|7|8|9)"},
    {"hex_digit" , "(0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|a|b|c|d|e|f)"}
};

std::map<string, string> literals =
{
    {"hex_lit","0(x|X)("+basics["hex_digit"]+")+"},
    {"decimal_lit","("+basics["digit"]+")+"},
    {"int_lit" , basics["hex_lit"]+"|"+basics["decimal_lit"]},
    {"char_lit" , basics["letter"]+"|"+basics["digit"]},
    {"string_lit", "("+basics["char_lit"]+")+"}
};

std::map<string, string> tokens =
{
    {"T_AND"           , "&&"},
    {"T_ASSIGN"        , "="},
    {"T_BOOLTYPE"      , "bool"},
    {"T_BREAK"         , "break"},
    {"T_CHARCONSTANT"  , literals["char_lit"]},
    {"T_COMMA"         , ","},
    {"T_COMMENT"       , "comment"},
    {"T_CONTINUE"      , "continue"},
    {"T_DIV"           , "/"},
    {"T_DOT"           , "."},
    {"T_ELSE"          , "else"},
    {"T_EQ"            ,  "=="},
    {"T_EXTERN"        , "extern"},
    {"T_FALSE"         , "false"},
    {"T_FOR"           , "for"},
    {"T_FUNC"          , "func"},
    {"T_GEQ"           , ">="},
    {"T_GT"            , ">"},
    {"T_ID"            , basics["letter"]+"("+basics["letter"]+"|"+basics["digit"]+")"},
    {"T_IF"            , "if"},
    {"T_INTCONSTANT"   , literals["int_lit"]},
    {"T_INTTYPE"       , "int"},
    {"T_LCB"           , "{"},
    {"T_LEFTSHIFT"     , "<<"},
    {"T_LEQ"           , "<="},
    {"T_LPAREN"        , "/("},
    {"T_LSB"           , "["},
    {"T_LT"            , "<"},
    {"T_MINUS"         , "-"},
    {"T_MOD"           , "%"},
    {"T_MULT"          , "/*"},
    {"T_NEQ"           , "!="},
    {"T_NOT"           , "!"},
    {"T_NULL"          , "null"},
    {"T_OR"            , "/|/|"},
    {"T_PACKAGE"       , "package"},
    {"T_PLUS"          , "/+"},
    {"T_RCB"           , "}"},
    {"T_RETURN"        , "return"},
    {"T_RIGHTSHIFT"    , ">>"},
    {"T_RPAREN"        , "/)"},
    {"T_RSB"           , "]"},
    {"T_SEMICOLON"     , ";"},
    {"T_STRINGCONSTANT", literals["string_lit"]},
    {"T_STRINGTYPE"    , "string"},
    {"T_TRUE"          , "true"},
    {"T_VAR"           , "var"},
    {"T_VOID","void"},
    {"T_WHILE", "while"}
};


