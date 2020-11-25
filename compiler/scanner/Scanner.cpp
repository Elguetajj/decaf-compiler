#include"Regx.cpp"
#include"TokenStream.cpp"
#include"ErrorStream.cpp"
#include<fstream>
#include<sstream>
#include<iostream>
#include <map>

Regx rreg;

std::map<string, string> basics =
{
    {"letter" , "(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|:|/)|/()"},
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

State* T_AND = rreg.compile("&&");
State* T_CLASS = rreg.compile("class");
State* T_ASSIGN = rreg.compile("=");
State* T_BOOLTYPE = rreg.compile("bool");
State* T_BREAK = rreg.compile("break");
State* T_LEFTCHAR = rreg.compile("\'("+literals["char_lit"]+")");
State* T_CHARCONSTANT = rreg.compile("\'("+literals["char_lit"]+")\'");
State* T_COMMA = rreg.compile(",");
State* T_COMMENT = rreg.compile("comment");
State* T_CONTINUE = rreg.compile("continue");
State* T_DIV =  rreg.compile("//");
State* T_DOT = rreg.compile("/.");  
State* T_ELSE = rreg.compile("else");
State* T_EQ = rreg.compile("==");
State* T_EXTERN = rreg.compile("extern");
State* T_FALSE = rreg.compile("false");
State* T_FOR = rreg.compile("for");
State* T_FUNC = rreg.compile("func");
State* T_GEQ = rreg.compile(">=");
State* T_GT = rreg.compile(">");
State* T_ID = rreg.compile("(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_)(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|a|b|c|d|e|f)*"); 
State* T_IF = rreg.compile("if");
State* T_INTCONSTANT = rreg.compile(literals["int_lit"]);
State* T_INTTYPE = rreg.compile("int");
State* T_LCB = rreg.compile("{");
State* T_LEFTSHIFT = rreg.compile("<<");
State* T_LEQ = rreg.compile("<=");
State* T_LPAREN = rreg.compile("/(");
State* T_LSB = rreg.compile("[");
State* T_LT = rreg.compile("<");
State* T_MINUS = rreg.compile("-");
State* T_MOD = rreg.compile("%");
State* T_MULT = rreg.compile("/*");
State* T_NEQ = rreg.compile("!=");
State* T_NOT = rreg.compile("!");
State* T_NULL = rreg.compile("null");
State* T_OR = rreg.compile("/|/|");
State* T_PACKAGE = rreg.compile("package");
State* T_PLUS = rreg.compile("/+");
State* T_RCB = rreg.compile("}");
State* T_RETURN = rreg.compile("return");
State* T_RIGHTSHIFT = rreg.compile(">>");
State* T_RPAREN = rreg.compile("/)");
State* T_RSB = rreg.compile("]");
State* T_SEMICOLON = rreg.compile(";");
State* T_QUOTE = rreg.compile("\"|\'");
State* T_LEFTSTRING = rreg.compile("\"("+literals["string_lit"]+"| )*");
State* T_STRINGCONSTANT = rreg.compile("\"("+literals["string_lit"]+"| )*\"");
State* T_STRINGTYPE = rreg.compile("string");
State* T_TRUE = rreg.compile("true");
State* T_VAR = rreg.compile("var");
State* T_VOID = rreg.compile("void");
State* T_WHILE = rreg.compile("while");
State* T_WHITESPACE = rreg.compile("(\n|\r|\t|\v|\f| )+");






class Scanner
{
public:
    TokenStream tokenize(char filename[],bool debug);
    TokenStream tokenstream;
    ErrorStream errorstream;

};

TokenStream Scanner::tokenize(char filename[], bool debug)
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
    int line=1;
    int position=1;

    int errline=0;
    int errposition=0;
    std::string charac;

    for(p=str; *p; p++){
        lexeme += *p;
        charac = *p;

        // cout<<lexeme<<"\n";
        if (rreg.eval(T_AND,lexeme))
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
        else if (rreg.eval(T_CLASS,lexeme))
        {
            type = "T_CLASS"; 
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
        else if (rreg.eval(T_ASSIGN,lexeme))
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
        else if (rreg.eval(T_BOOLTYPE,lexeme))
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
        else if (rreg.eval(T_BREAK,lexeme))
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
        else if (rreg.eval(T_CHARCONSTANT,lexeme))
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
         else if (rreg.eval(T_LEFTCHAR,lexeme))
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
        else if (rreg.eval(T_COMMA,lexeme))
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
        else if (rreg.eval(T_DIV,lexeme))
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
        else if (rreg.eval(T_DOT,lexeme))
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
        else if (rreg.eval(T_ELSE,lexeme))
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
        else if (rreg.eval(T_EQ,lexeme))
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
        else if (rreg.eval(T_EXTERN,lexeme))
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
        else if (rreg.eval(T_FALSE,lexeme))
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
        else if (rreg.eval(T_FOR,lexeme))
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
        else if (rreg.eval(T_FUNC,lexeme))
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
        else if (rreg.eval(T_GEQ,lexeme))
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
        else if (rreg.eval(T_GT,lexeme))
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
        else if (rreg.eval(T_IF,lexeme))
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
        else if (rreg.eval(T_INTTYPE,lexeme))
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
        else if (rreg.eval(T_LCB,lexeme))
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
        else if (rreg.eval(T_LEFTSHIFT,lexeme))
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
        else if (rreg.eval(T_LEQ,lexeme))
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
        else if (rreg.eval(T_LPAREN,lexeme))
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
        else if (rreg.eval(T_LSB,lexeme))
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
        else if (rreg.eval(T_LT,lexeme))
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
        else if (rreg.eval(T_MINUS,lexeme))
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
        else if (rreg.eval(T_MOD,lexeme))
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
        else if (rreg.eval(T_MULT,lexeme))
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
        else if (rreg.eval(T_NEQ,lexeme))
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
        else if (rreg.eval(T_NOT,lexeme))
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
        else if (rreg.eval(T_NULL,lexeme))
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
        else if (rreg.eval(T_OR,lexeme))
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
        else if (rreg.eval(T_PACKAGE,lexeme))
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
        else if (rreg.eval(T_PLUS,lexeme))
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
        else if (rreg.eval(T_RCB,lexeme))
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
        else if (rreg.eval(T_RETURN,lexeme))
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
        else if (rreg.eval(T_RIGHTSHIFT,lexeme))
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
        else if (rreg.eval(T_RPAREN,lexeme))
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
        else if (rreg.eval(T_RSB,lexeme))
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
        else if (rreg.eval(T_SEMICOLON,lexeme))
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
        else if (rreg.eval(T_TRUE,lexeme))
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
        else if (rreg.eval(T_VAR,lexeme))
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
        else if (rreg.eval(T_VOID,lexeme))
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
        else if (rreg.eval(T_WHILE,lexeme))
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
        else if (rreg.eval(T_CONTINUE,lexeme))
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
        else if (rreg.eval(T_BOOLTYPE,lexeme))
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
        else if (rreg.eval(T_STRINGCONSTANT,lexeme))
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
         else if (rreg.eval(T_LEFTSTRING,lexeme))
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
        else if (rreg.eval(T_QUOTE,lexeme))
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
        else if (rreg.eval(T_INTCONSTANT,lexeme))
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
        else if (rreg.eval(T_ID,lexeme))
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
        else if (rreg.eval(T_WHITESPACE,lexeme))
        {
            // cout<<"possible"+ type+":" <<lexeme<<"\n";
            if(charac == "\n"){
                line++;
                position = 0;
            }
            if(errlex.size()>0)
            {
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
                    // cout<<rreg.eval(T_ID,lexeme)<<"\n";
                    errline = line;
                    errposition = position;
                    errlex += lexeme;
                    lexeme ="";  
                } 
            }
        }
        position++;
    }
    if(tokenstream.head!=NULL && debug)
    { 
        cout<<"Token Stream:"<<"\n";
        tokenstream.printTokenStream();
    }
    if(errorstream.head!=NULL)
    {
        cout<<"\n"<<"Error Stream:"<<"\n";
        errorstream.printErrorStream();
    }
    return this->tokenstream;
}
