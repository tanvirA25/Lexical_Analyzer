#include "lex.h"
#include <map>

#include <iterator>


using namespace std;


   map <string, Token> tokString = {
		{"PROGRAM", PROGRAM},
		{"PRINT", PRINT},
		{"INT", INT},
		{ "END", END },
		{ "FLOAT", FLOAT},
		{ "BOOL", BOOL },
		
		{ "ELSE", ELSE },
		{ "IF", IF },
		
		{"THEN", THEN},
		{ "TRUE", TRUE },
		{ "FALSE", FALSE },
		
		{ "IDENT", IDENT },

		{ "ICONST", ICONST },
		{ "RCONST", RCONST },
		{ "SCONST", SCONST },
		{ "BCONST", BCONST },
		
		{ "PLUS", PLUS },
		{ "MINUS", MINUS },
		{ "MULT", MULT },
		{ "DIV", DIV },
		{ "ASSOP", ASSOP},
		{ "EQUAL", EQUAL },
		{ "GTHAN", GTHAN },
		{ "LTHAN", LTHAN },
		{ "AND", AND },
		{ "OR", OR },
		{ "NOT", NOT },
            
		{ "COMMA", COMMA},
		{ "LPAREN", LPAREN },
		{ "RPAREN", RPAREN },
		
		
		{ "SEMICOL", SEMICOL },
		
		{ "ERR", ERR },

		{ "DONE", DONE },
	};
   

LexItem id_or_kw(const string& lexeme, int linenum){
 map<string, Token>::iterator it = tokString.find(lexeme);


    
    if(it != tokString.end()){
       if(tokString.find(lexeme)->second == FALSE || tokString.find(lexeme)->second == TRUE){
            
            return LexItem(BCONST, lexeme, linenum);
        }
        else
            return LexItem(it->second, lexeme, linenum);
    }
    else 
   
        return LexItem(IDENT, lexeme, linenum);
    
}


ostream& operator<<(ostream& out, const LexItem&tok) {
        string lex = tok.GetLexeme();
   
   int line_num = tok.GetLinenum();
        
        if (tok.GetToken() == IDENT){
            
            if (lex == "PROGRAM" || lex == "BOOL" || lex == "FALSE" || lex == "TRUE" ||  lex == "IF" || lex == "THEN" || lex ==  "ELSE" || lex == "END" ||lex == "INT" || tok.GetLexeme() == "FLOAT" || lex == "PRINT"){
                out <<"KEYWORD: " <<lex << " at Line "<< line_num << endl;
        }
        
        else
            return out <<"IDENT: " <<lex << " at Line "<< line_num << endl;
         
        
    }
			
        
        else if(tok.GetToken()  == ICONST){
			return out <<"ICONST: "  << "(" <<lex <<")"<<  " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == RCONST){
			return out <<"RCONST: "  << "(" <<lex <<")"<<  " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == SCONST){
			return out <<"SCONST: " << '"'<<lex <<'"'<<  " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == BCONST){
			return out <<"BCONST: " << "(" <<lex <<")"<< " at Line "<< line_num << endl;
        }
			
        else if(tok.GetToken()  == PLUS){
			return out <<"PLUS: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == MINUS){
			return out <<"MINUS: "<< "'"<<lex <<"'" << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == MULT){
			return out <<"MULT: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == DIV){
			return out <<"DIV: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == ASSOP){
			return out <<"ASSOP: " << "'"<<lex <<"'"<< " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == EQUAL){
			return out <<"EQUAL: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == GTHAN){
			return out <<"GTHAN: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == LTHAN){
			return out <<"LTHAN: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == EQUAL){
			return out <<"EQUAL: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == AND){
			return out <<"AND: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == OR){
			return out <<"OR: " <<lex << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == NOT){
			return out <<"NOT: " <<lex<< " at Line "<< line_num << endl;
        }
			
        else if(tok.GetToken()  == COMMA){
			return out <<"COMMA: " << "'"<<lex <<"'"<< " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == LPAREN){
			return out <<"LPAREN: "  <<"'" <<lex << "'" << " at Line "<< line_num << endl;
        }
        else if(tok.GetToken()  == RPAREN){
			return out <<"RPAREN: " << "'"<<lex << "'"<< " at Line "<< line_num << endl;
        }
			
        else if(tok.GetToken()  == SEMICOL){
			return out <<"SEMICOL: "<< "'"<<lex <<"'"<< " at Line "<< line_num << endl;
        }
			
        else if(tok.GetToken()  == ERR){
			return out <<"Error: " <<": "<< '"'<<lex <<'"'<< " at Line "<< line_num << endl;
        }
			
        else if(tok.GetToken()  == DONE){
			return out <<"DONE: " <<lex << " at Line "<< tok.GetLinenum() << endl;
        }
        


 
   

    return out;
}

LexItem getNextToken(istream& in, int& linenum) {
    
 enum TokenState {START, INID, INSTRING, ININT, INREAL,INCOMMENT} 
    lexstate = START;
    string lexeme; 
    char ch;
        
   while(in.get(ch))
    {
        switch(lexstate) {
               
            case START:
                if(ch == '\n')
                {
                    linenum++;
                }
                
                else if (isspace(ch))
                    continue;
                  
                
                // comment checker
                else if(ch == '/' && char(in.peek()) == '/')
                {
                    lexstate = INCOMMENT;
                    continue;
                }
                
                // if end of file         
                else if(in.peek()==-1)
                {
                    return LexItem(DONE, lexeme, linenum);
                }
                else if(isalpha(ch) || ch == '_')
                {
                    lexeme += toupper(ch);
                    lexstate = INID;
                }
                else if(isdigit(ch))
                {
                    lexeme += ch;
                   lexstate = ININT;
                }
                else if(ch == '!')
                {
                    
                    return LexItem(NOT, lexeme, linenum);
                   
                }
                else if(ch == '.' && isdigit(in.peek()))
                {
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                 
                }
                else if(ch == '/' && in.peek() == '*')
                {
                    lexstate = INCOMMENT;
                    lexeme += ch;
                }
                else if(ch == '\"')
                {
                    lexstate = INSTRING;
                    lexeme += '\"';
                }
                else if(ch == '+')
                {
                 
                    return LexItem(PLUS, lexeme, linenum);
                    
                }
                else if(ch == '-')
                {
                  
                     return LexItem(MINUS, lexeme, linenum);
                    
                }
                else if(ch == '*')
                {
                   
                     return LexItem(MULT, lexeme, linenum);
                    
                }
                else if(ch == '/')
                {
                 
                     return LexItem(DIV, lexeme, linenum);
                       
                }
                else if(ch == '=' &&in.peek()== '=')
                {
                
                    in.ignore(1);
                    return LexItem(EQUAL, lexeme, linenum);
                }

               else if(ch == '=')
                {

                    if(in.peek() == '=')
                    {
                        
                        in.ignore(1);
                        LexItem item(EQUAL, lexeme, linenum);
                        return item;
                    }
                    else
                    {
                        LexItem item(ASSOP, lexeme, linenum);
                        return item;
                    }


                }
                 else if(ch == '|' && in.peek() == '|')
                {
                    in.ignore(1);
                    lexeme += ch;
                    return LexItem(OR, lexeme, linenum);
                     
                }
                else if(ch == '(')
                {
                 
                     return LexItem(LPAREN, lexeme, linenum);
                     
                }
                else if(ch == ')')
                {
                    
                     return LexItem(RPAREN, lexeme, linenum);
                      
                }
                else if(ch == ';')
                {
                   
                     return LexItem(SEMICOL, lexeme, linenum);
                      
                }
                else if(ch == ',')
                {
                 
                     return LexItem(COMMA, lexeme, linenum);
                   
                }
                else if(ch == '>')
                {
                    
                     return LexItem(GTHAN, lexeme, linenum);
                      
                }
                else if(ch == '<')
                {
                 
                     return LexItem(LTHAN, lexeme, linenum);
                     
                }
                
                
                else if(ch == '&' && in.peek() == '&')
                {
                    in.ignore(1);
                    lexeme += ch;
                     return LexItem(AND, lexeme, linenum);
                   
                }
                else
                {
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                     
                }
                break;
               
            case INID:
                if(isalnum(ch) || ch == '@' || ch == '_')
                {
                    lexeme +=toupper(ch);
                }
                else{
                    in.putback(ch);
                     return LexItem(id_or_kw(lexeme, linenum));
                    
                }
                break;
               
           
            case ININT:
                
                if (isalpha(ch)){
                     in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                    
                }
                if (ch == '_'){
                     in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                }
                 if (ch == '*')
                {
                    in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                    
                }
                if (ch == '+'){
                    in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                    
                    }
                if (ch == '-'){
                     in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                    
                }
                if ( ch == '='){
                     in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                }
                if(ch == '/'){
                     in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                }
                if(ch == ','){
                     in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                    
                }
                if(ch == ')'){
                     in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                   }
                if(ch == ';'){
                     in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                   }
                
                if (ch == '\n' ){
                     linenum++;
                    
                     return LexItem(ICONST, lexeme, linenum);
                    
                }
                if (ch == ' '){
                    return LexItem(ICONST, lexeme, linenum);
                     
                }
                else if (ch == '.' && isdigit(in.peek()))
                {
                    lexstate = INREAL;
                    lexeme += ch;
                }
                else if(isdigit(ch))
                {
                    lexeme += ch;
                }
                else
                {
                    lexeme += ch;
                     return LexItem(ERR, lexeme, linenum);
                    
                }
                break;
                 
            case INREAL:
                if (ch == '*')
                {
                    in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                    
                }
                if (ch == '+'){
                    in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                    
                    }
                if (ch == '-'){
                     in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                    
                }
                if ( ch == '='){
                     in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                }
                if(ch == '/'){
                     in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                }
                if(ch == ','){
                     in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                    
                }
                if (ch == '='){
                     in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                }
                if(ch == ')'){
                     in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                   }
                if(ch == ';'){
                     in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                   }
                
                else if (ch == '\n')
                {
                    linenum++;
                    return LexItem(RCONST, lexeme, linenum);
                     
                 
                }
                if (ch == ' '){
                    return LexItem(RCONST, lexeme, linenum);
                }
                else if (isdigit(ch))
                {
                    lexeme += ch;
                }
                else
                {
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                   
                }
               
                break;
                 
            case INSTRING:
                
                if (ch == '\n')
                {
                    return LexItem(ERR, lexeme, linenum);
                   
                }
                if (ch == '\"' )
                {
                    lexeme += ch;
                    return LexItem(SCONST, lexeme, linenum);
                   
                }

                lexeme += ch;
                break;
                   
            case INCOMMENT:
                if(ch == '*' && in.peek()=='/')
                {
                    in.ignore(1);
                    lexeme = "";
                    lexstate = START;
                    break;
                }
                if(in.peek() == EOF)
                {
                    return LexItem(ERR, lexeme, linenum);
                    
                }
                if(ch == '\n')
                {
                    linenum++;
                }
               
                lexeme += ch;
               
                break;
               
        }

    }
   
    LexItem item(DONE, lexeme, linenum);
    return item;
}




