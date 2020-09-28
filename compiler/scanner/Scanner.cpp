#include"Regx.cpp"
#include"TokenStream.cpp"
#include"ErrorStream.cpp"
#include<fstream>
#include<sstream>
#include<iostream>
#include <map>

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
    {"int_lit" , "(0(x|X)("+basics["hex_digit"]+")+)"+"|"+"(("+basics["digit"]+")("+basics["digit"]+")*)"},
    {"char_lit" , basics["letter"]+"|"+basics["digit"]},
    {"string_lit", "("+basics["letter"]+"|"+basics["digit"]+")"}
};

std::map<string, string> tokens =
{
    {"T_AND"           , "&&"},//
    {"T_ASSIGN"        , "="},//
    {"T_BOOLTYPE"      , "bool"},//
    {"T_BREAK"         , "break"},//
    {"T_LEFTCHAR", "\'("+literals["char_lit"]+")"},
    {"T_CHARCONSTANT", "\'("+literals["char_lit"]+")\'"},
    {"T_COMMA"         , ","},
    {"T_COMMENT"       , "comment"},
    {"T_CONTINUE"      , "continue"},
    {"T_DIV"           , "//"},
    {"T_DOT"           , "/."},  
    {"T_ELSE"          , "else"},
    {"T_EQ"            ,  "=="},
    {"T_EXTERN"        , "extern"},
    {"T_FALSE"         , "false"},
    {"T_FOR"           , "for"},
    {"T_FUNC"          , "func"},
    {"T_GEQ"           , ">="},
    {"T_GT"            , ">"},
    {"T_ID"            , "(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_)(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|a|b|c|d|e|f)*"}, //
    {"T_IF"            , "if"},
    {"T_INTCONSTANT"   , literals["int_lit"]},
    {"T_INTTYPE"       , "int"},
    {"T_LCB"           , "{"},
    {"T_LEFTSHIFT"     , "<<"},
    {"T_LEQ"           , "<="},
    {"T_LPAREN"        , "///("},
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
    {"T_RPAREN"        , "///)"},
    {"T_RSB"           , "]"},
    {"T_SEMICOLON"     , ";"},
    {"T_QUOTE"         , "\"|\'"},
    {"T_LEFTSTRING", "\""+literals["string_lit"]+"*"},
    {"T_STRINGCONSTANT", "\""+literals["string_lit"]+"*\""},
    {"T_STRINGTYPE"    , "string"},
    {"T_TRUE"          , "true"},
    {"T_VAR"           , "var"},
    {"T_VOID","void"},
    {"T_WHILE", "while"},
    {"T_WHITESPACE","(\n|\r|\t|\v|\f| )+"}
};

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
    std::string tokenValue = "";
    std::string type;
    int line=0;
    int position=1;

    int errline=0;
    int errposition=0;
    std::string charac;

    for(p=str; *p; p++){
        lexeme += *p;
        charac = *p;
        // cout<<lexeme<<"\n";
        if (re.eval(tokens["T_AND"],lexeme))
        {
            type = "T_AND"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_ASSIGN"],lexeme))
        {
            type = "T_ASSIGN"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_BOOLTYPE"],lexeme))
        {
            type = "T_BOOLTYPE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_BREAK"],lexeme))
        {
            type = "T_BREAK"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_CHARCONSTANT"],lexeme))
        {
            type = "T_CHARCONSTANT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
         else if (re.eval(tokens["T_LEFTCHAR"],lexeme))
        {
            type = "T_LEFTCHAR"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                // lexeme="";
                // if (strlen(p)==1){
                //     tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                // }
            } 
        }
        else if (re.eval(tokens["T_COMMA"],lexeme))
        {
            type = "T_COMMA"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_DIV"],lexeme))
        {
            type = "T_DIV"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_DOT"],lexeme))
        {
            type = "T_DOT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_ELSE"],lexeme))
        {
            type = "T_ELSE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_EQ"],lexeme))
        {
            type = "T_EQ"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_EXTERN"],lexeme))
        {
            type = "T_EXTERN"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_FALSE"],lexeme))
        {
            type = "T_FALSE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_FOR"],lexeme))
        {
            type = "T_FOR"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_FUNC"],lexeme))
        {
            type = "T_FUNC"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_GEQ"],lexeme))
        {
            type = "T_GEQ"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_GT"],lexeme))
        {
            type = "T_GT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_IF"],lexeme))
        {
            type = "T_IF"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_INTTYPE"],lexeme))
        {
            type = "T_INTTYPE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_LCB"],lexeme))
        {
            type = "T_LCB"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_LEFTSHIFT"],lexeme))
        {
            type = "T_LEFTSHIFT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_LEQ"],lexeme))
        {
            type = "T_LEQ"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_LPAREN"],lexeme))
        {
            type = "T_LPAREN"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_LSB"],lexeme))
        {
            type = "T_LSB"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_LT"],lexeme))
        {
            type = "T_LT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_MINUS"],lexeme))
        {
            type = "T_MINUS"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }

        else if (re.eval(tokens["T_MOD"],lexeme))
        {
            type = "T_MOD"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_MULT"],lexeme))
        {
            type = "T_MULT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_NEQ"],lexeme))
        {
            type = "T_NEQ"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_NOT"],lexeme))
        {
            type = "T_NOT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_NULL"],lexeme))
        {
            type = "T_NULL"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_OR"],lexeme))
        {
            type = "T_OR"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_PACKAGE"],lexeme))
        {
            type = "T_PACKAGE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_PLUS"],lexeme))
        {
            type = "T_PLUS"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_RCB"],lexeme))
        {
            type = "T_RCB"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_RETURN"],lexeme))
        {
            type = "T_RETURN"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_RIGHTSHIFT"],lexeme))
        {
            type = "T_RIGHTSHIFT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_RPAREN"],lexeme))
        {
            type = "T_RPAREN"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_RSB"],lexeme))
        {
            type = "T_RSB"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_SEMICOLON"],lexeme))
        {
            type = "T_SEMICOLON"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_TRUE"],lexeme))
        {
            type = "T_TRUE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_VAR"],lexeme))
        {
            type = "T_VAR"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_VOID"],lexeme))
        {
            type = "T_VOID"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_WHILE"],lexeme))
        {
            type = "T_WHILE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_CONTINUE"],lexeme))
        {
            type = "T_CONTINUE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_BOOLTYPE"],lexeme))
        {
            type = "T_BOOLTYPE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_STRINGCONSTANT"],lexeme))
        {
            type = "T_STRINGCONSTANT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
         else if (re.eval(tokens["T_LEFTSTRING"],lexeme))
        {
            type = "T_LEFTSTRING"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                // lexeme="";
                // if (strlen(p)==1){
                //     tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                // }
            } 
        }
        else if (re.eval(tokens["T_QUOTE"],lexeme))
        {
            type = "T_QUOTE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_INTCONSTANT"],lexeme))
        {
            type = "T_INTCONSTANT"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
            if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            } 
        }
        else if (re.eval(tokens["T_ID"],lexeme))
        {
            type = "T_ID"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
            if (lexeme.size()>tokenValue.size())
            {
                tokenValue = lexeme;
                if (strlen(p)==1){
                    tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                }
            }   
        }
        else if (re.eval(tokens["T_WHITESPACE"],lexeme))
        {
            type = "T_WHITESPACE"; 
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(errlex.size()>0){
                errorstream.addError(new Error(errlex,errposition,errline));
                errlex = "";
            }
             if (lexeme.size()>tokenValue.size())
            {
                lexeme="";
                // if (strlen(p)==1){
                //     tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line));
                // }
            } 
        }
        else
        {
            if(tokenValue.size()>0)
            {
                tokenstream.addToken(new Token(type,tokenValue,position-tokenValue.size(),line)); 
                tokenValue = "";
                lexeme = "";
                position--;
                p--;
            }
            else
            {
                if (strlen(p)==1)
                {
                    errline = line;
                    errposition = position;
                    errlex += lexeme;
                    errorstream.addError(new Error(errlex,errposition,errline));
                }
                else
                {
                    // cout<<"possible error:"<<lexeme<<"\n";
                    // cout<<re.eval(tokens["T_ID"],lexeme)<<"\n";
                    errline = line;
                    errposition = position;
                    errlex += lexeme;
                    lexeme ="";  
                } 
            }
        }
        position++;
    }
    if(tokenstream.head!=NULL)
    { cout<<"Token Stream:"<<"\n";
        tokenstream.printTokenStream();
    }
    if(errorstream.head!=NULL)
    {
        cout<<"\n"<<"Error Stream:"<<"\n";
        errorstream.printErrorStream();
    }
}

