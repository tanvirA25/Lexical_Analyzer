/* Implementation of Recursive-Descent Parser
 * parseInt.cpp
 * Programming Assignment 3
 * Fall 2022
*/

#include "parseInt.h"

map<string, bool> defVar;
queue <Value> *ValQue;
map<string, Value> TempsResults;
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = f/* Implementation of Recursive-Descent Parser
 * parse.cpp
 * Programming Assignment 2
 * Fall 2022
*/

#include "parseInt.h"

map<string, bool> defVar;
queue <Value> *ValQue;
map<string, Value> TempsResults;
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(Token dataType, Value &val){
  switch(dataType){
    case INT:
    if(val.IsReal()){
      val = Value((int)(val.GetReal()));
    }
    return val.IsInt() || val.IsReal();

    case BOOL:
    return val.IsBool();

    case FLOAT:
    if(val.IsInt()){
      val = Value((float)(val.GetInt()));
    }
    return val.IsReal() || val.IsInt();

    default:
    return false;
  }
}



extern bool Prog(istream &in, int &line) {

  bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
    
    
    
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
            f1 = StmtList(in, line); 
			if(f1) {
				tok = Parser::GetNextToken(in, line);
				if( tok == END){
					tok = Parser::GetNextToken(in, line);
					if(tok != PROGRAM){
						ParseError(line, "Missing PROGRAM Keyword.");
						return false;
					}
					return true;
				}
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
    else if(!PROGRAM || !f1)
    {
       ParseError(line, "Incorrect Program Body");
       return false; 
    }
    else if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
        }
    else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
        ParseError(line, "Empty File");
        return true;
        }
 
    return f1;
    
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
extern bool StmtList(istream &in, int &line) {
  bool status;

  LexItem t = Parser::GetNextToken(in, line);
  if (t == END) {
    return true;
  }

  Parser::PushBackToken(t);
  status = Stmt(in, line);
  t = Parser::GetNextToken(in, line);
  // cout << t.GetToken() << ", " << t.GetLexeme() << ", " << t.GetLinenum() <<
  // endl;
  if (t == PROGRAM && !status) {
    ParseError(line, "Syntactic error in Program Body.");
    return false;
  } else {
    Parser::PushBackToken(t);
  }
  return status;
}//End of StmtList function

//Stmt ::= DeclStmt | ControlStmt
extern bool Stmt(istream &in, int &line) {
  bool status=false;
	
	LexItem t = Parser::GetNextToken(in, line);
    
  
    
    if(t == END){
        return true;
        t = Parser::GetNextToken(in, line);
        if(t == PROGRAM){
            Parser::PushBackToken(t);
            return false;
        }
      
    }
	
	switch( t.GetToken() ) {
	case INT: case FLOAT: case BOOL:
		Parser::PushBackToken(t);
		status = DeclStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Declaration Statement.");
			return status;
		}
		break;
	case IF: case PRINT: case IDENT: 
		Parser::PushBackToken(t);
		status = ControlStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect control Statement.");
			return status;
		}
		break;
	default:
		Parser::PushBackToken(t);
        return true;
	}
    
    LexItem tok = Parser::GetNextToken(in,line);
    if(tok != SEMICOL){
        ParseError(line, "Missing semicolon at end of Statement.");
        return false;
        
    }
    else
        status = Stmt(in, line);
    
  
	return status;
}//End of Stmt function

//DeclStmt ::= ( INT | FLOAT | BOOL ) VarList
extern bool DeclStmt(istream &in, int &line) {
 LexItem t;
	bool status;
	
	t = Parser::GetNextToken(in, line);
	if(t == INT || t == FLOAT || t == BOOL)
	{
		status = VarList(in, line, t);
		
		if (!status)
		{
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return status;
		}
        
        
	}
	else
	{
		ParseError(line, "Incorrect Declaration Type.");
		return false;
	}
	
	return true;
}//End of DeclStmt

//VarList:= Var {,Var}
extern bool VarList(istream &in, int &line, LexItem &type) {
 bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
            SymTable[identstr] = type.GetToken();
            
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = VarList(in, line, type);
	}
	else {
		Parser::PushBackToken(tok);
		return true;
	}
	return status;	
}//End of VarList

//ControlStmt ::= AssigStmt | IfStmt | PrintStmt 
extern bool ControlStmt(istream &in, int &line) {
 bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		break;
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
} // End of ControlStmt

//PrintStmt:= PRINT (ExpreList) 
bool PrintStmt(istream &in, int &line) {
  LexItem t;
    
    ValQue = new queue<Value>;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
        
        ParseError(line, "Missing expression list after Print");
        while(!(*ValQue).empty()){
            ValQue->pop();
        }
        delete ValQue;
        

		return false;
	}
	
    
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
    while (!(*ValQue).empty()){
        cout << (*ValQue).front();
       
        
        if( (*ValQue).front().IsErr()){
            ParseError(line, "Undefined Variable");
            return false;
        }
         ValQue->pop();
    }
    cout<< endl;
	return ex;
}//End of PrintStmt

//IfStmt:= IF (Expr) then StmtList [Else StmtList] END IF
extern bool IfStmt(istream &in, int &line) {
    bool status;
  LexItem t = Parser::GetNextToken(in, line);

  if (t != LPAREN) {
    ParseError(line, "Missing Left Parenthesis");
    return false;
  }

  Value val;
  status = Expr(in, line, val);
  if (!status) {
    ParseError(line, "Missing if statement Logic Expression");
    return false;
  }

  t = Parser::GetNextToken(in, line);
  if (t != RPAREN) {

    ParseError(line, "Missing Right Parenthesis");
    return false;
  }

  t = Parser::GetNextToken(in, line);
  if (t != THEN) {
    // ParseError(line, "Missing expression list after Then");
    return false;
  }

  if(val.IsBool() && val.GetBool()){
      status = StmtList(in, line);
      if (!status) {
        // ParseError(line, "If Statement Syntax Error");
        ParseError(line, "Missing Statement for If-Stmt Then-Part");
        return false;
      }
  }
  else if (val.IsBool())
  {
      while(t != ELSE && t != END){
        t = Parser::GetNextToken(in, line);
      }
      Parser::PushBackToken(t);
    }
  else{
    ParseError(line, "Missing Statement for If-Stmt If-Part");
    return false;
  }

  t = Parser::GetNextToken(in, line);
  if (t == ELSE && val.GetBool()) {
      while(t != END){
        t = Parser::GetNextToken(in, line);
      }
    }
  else if (t == ELSE){
     status = StmtList(in, line);
      if (!status) {
        ParseError(line, "Missing expression lidfst after Else");
        return false;
      }
    }
  else {
    Parser::PushBackToken(t);
  }
  
  t = Parser::GetNextToken(in, line);
  if(t == END){
    t = Parser::GetNextToken(in, line);
      if (t != IF) {
        ParseError(line, "Missing closing keywords of IF statement.");
        return false;
      }
  }
  return true;

	
}//End of IfStmt function

//Var:= ident
extern bool Var(istream &in, int &line, LexItem &idtok) {
   bool status;
   string identstr;
  LexItem tok = Parser::GetNextToken(in, line);
  if (tok == IDENT) {
    identstr = tok.GetLexeme();
    status = defVar.find(identstr)->second;
    if (!status) {
       ParseError(line, "Variable Redefinition");
       return false;
      }
  }
  else if(tok.GetToken() == ERR){
      ParseError(line, "Unrecognized Input Pattern");
      cout << "(" << tok.GetLexeme() << ")" << endl;
      return false;
}
    
  else {
      ParseError(line, "Invalid Variable.");
        return false;
}
 
  return status;

}//End of Var
    
//AssignStmt:= Var = Expr
extern bool AssignStmt(istream &in, int &line) {
    LexItem tok = Parser::GetNextToken(in, line);
    
  if (defVar.count(tok.GetLexeme()) != 1) {
    ParseError(line, "Undeclared Variable");
    return false;
  }
  LexItem t = Parser::GetNextToken(in, line);
  
  Value val1;
  bool status = Expr(in, line, val1);
  if( t== ASSOP){
      if(status){
        Token dataType = SymTable[tok.GetLexeme()];
        if(val1.IsErr()){
          ParseError(line, "Missing Expression in Assignment Statement");
          return false;
        }
        if(IdentList(dataType, val1)){
          TempsResults[tok.GetLexeme()] = val1;
        }else{
          ParseError(line, "Illegal Assignment Operation");
          return false;
        }
      }}
  else{
    ParseError(line, "Missing assignment operator");
    return false;
  }
  return status;
}//End of AssignStmt

//ExprList:= Expr {,Expr}
extern bool ExprList(istream &in, int &line) {
  bool status = false;
	Value retVal;
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
    ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
		
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
} // End of ExprList

//Expr ::= LogORExpr ::= LogANDExpr { || LogANDRxpr }
extern bool Expr(istream &in, int &line, Value &retVal) {
    Value val1, val2;
	LexItem tok;
	bool t1 = LogANDExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
    
    retVal = val1;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == OR ) 
	{
		t1 = LogANDExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
        
        retVal = retVal || val2;
        if(retVal.IsErr()){
            ParseError(line, "Illegal OR operation.");
            return false;
        }
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr/LogORExpr

// LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream &in, int &line, Value &retVal) {
  
    Value val1, val2;
	LexItem tok;
	bool t1 = EqualExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	
    retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == AND ) 
	{
		t1 = EqualExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
        
        retVal = retVal && val2;
        if (retVal.IsErr()){
            ParseError(line, "Illegal AND operation.");
            return false;
        }
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	
	Parser::PushBackToken(tok);
	return true;
} // End of LogANDExpr
    
//EqualExpr ::= RelExpr [ == RelExpr ]
extern bool EqualExpr(istream &in, int &line, Value &retVal) {
   Value val1, val2;
	LexItem tok;
	bool t1 = RelExpr(in, line, val1);
		
	if( !t1 ) {
        ParseError(line, "Missing operand after operator");
		return false;
	}
	
    retVal = val1;
    
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == EQUAL ) 
	{
		t1 = RelExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
        
        retVal = retVal == val2;
        if(retVal.IsErr()){
            ParseError(line, "Illegal EQUAL operation.");
            return false;
        }
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of EqualExpr
    
//RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
extern bool RelExpr(istream &in, int &line, Value &retVal) {
 
    Value val1, val2;
	LexItem tok;
	bool t1 = AddExpr(in, line, val1);
		
	if( !t1 ) {
        ParseError(line, "Missing operand after operator");
		return false;
	}
	
    retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == LTHAN || tok == GTHAN) 
	{
		t1 = AddExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
        
        retVal = (tok == GTHAN)? retVal > val2: retVal < val2;
		
        if(retVal.IsErr()){
            ParseError(line, (tok == GTHAN)? "Illegal Greather Than operation.": "Illegal Less Than operation");
            return false;
        }

		tok = Parser::GetNextToken(in, line);
		
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr
    
//AddExpr :: MultExpr { ( + | - ) MultExpr }
extern bool AddExpr(istream &in, int &line, Value &retVal) {
   Value val1, val2;
	
	bool t1 = MultExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
    retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = MultExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
       if(tok == PLUS){
            retVal = retVal + val2;
            if(retVal.IsErr()){
                ParseError(line,"Illegal Addition operation");
                return false;
            }
        }
        else if(tok == MINUS){
            retVal = retVal - val2;
            if(retVal.IsErr()){
                ParseError(line, "Illegal Subtraction operation");
                return false;
            }
        }
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr
    
//MultExpr ::= UnaryExpr { ( * | / ) UnaryExpr }
extern bool MultExpr(istream &in, int &line, Value &retVal) {
  Value val1, val2;
	
	bool t1 = UnaryExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
    retVal = val1;
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		t1 = UnaryExpr(in, line, val2);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
        if(tok == MULT){
            retVal = retVal * val2;
            if(retVal.IsErr()){
                ParseError(line,"Illegal multiplicaiton operation");
                return false;
            }
        }
        else if(tok == DIV ){
            if(val2.GetInt() ==0 || val2.GetReal() ==0){
                ParseError(line, "Run-Time Error-Illegal Division by Zero ");
                return false;
            }
        }
        
            //
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
    
	Parser::PushBackToken(tok);
    return true;
}//End of MultExpr
    
//UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
extern bool UnaryExpr(istream &in, int &line, Value &retVal) {
 
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = 1;
	}
	else if(t == PLUS)
	{
		sign = 2 ;
	}
    else if (t == NOT){
        sign = 3;
    }
	else
		Parser::PushBackToken(t);
		
	status = PrimaryExpr(in, line, sign, retVal);
   
	return status;
}//End of UnaryExpr
 
//PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | ( Expr )
extern bool PrimaryExpr(istream &in, int &line, int sign, Value &retVal) {
  LexItem tok = Parser::GetNextToken(in, line);
  if( tok == IDENT ) {
		
		if(SymTable[tok.GetLexeme()] == PROGRAM){
      ParseError(line, "Illegal use of program name as a variable");
      return false;
    }
    retVal = Value(TempsResults[tok.GetLexeme()]);
	}
	else if( tok == ICONST ) {
		
		retVal = Value(stoi(tok.GetLexeme()));
	}
	else if( tok == SCONST ) {
		
		retVal = Value(tok.GetLexeme());
	}
	else if( tok == RCONST ) {
		
		 retVal = Value(stof(tok.GetLexeme()));
	}
	else if( tok == BCONST ) {
		
		retVal = (tok.GetLexeme() == "TRUE") ? Value(true) : Value(false);
	}
	
  
  bool ex;
  if (tok == LPAREN) {
    Value val1;
    ex = Expr(in, line, val1);
    if (!ex) {
      ParseError(line, "Missing expression list after Print");
      return false;
    }
    tok = Parser::GetNextToken(in, line);
    if (tok != RPAREN) {

      ParseError(line, "Missing Right Parenthesis");
      return false;
    }
    retVal = val1;
  }
    
  if(sign  ==1){
    if(retVal.IsInt()){
      retVal = Value(-retVal.GetInt());
      return true;
    }else if(retVal.IsReal()){
      retVal = Value(-retVal.GetReal());
      return true;
    }else{
      ParseError(line, "Illegal Operand Type for Sign Operator");
      return false;
    }
  }

  else if (sign == 2){
    if(!(retVal.IsInt() || retVal.IsReal())){
      ParseError(line, "Illegal Operand Type for Sign Operator");
      return false;
    }
    return true;
}
  else if (sign == 3) {
    if(retVal.IsBool()){
      retVal = Value(!retVal);
      return true;
    }
    return false;
  }

  return true;
}alse;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(Token dataType, Value &val){
  switch(dataType){
    case INT:
    if(val.IsReal()){
      val = Value((int)(val.GetReal()));
    }
    return val.IsInt() || val.IsReal();

    case BOOL:
    return val.IsBool();

    case FLOAT:
    if(val.IsInt()){
      val = Value((float)(val.GetInt()));
    }
    return val.IsReal() || val.IsInt();

    default:
    return false;
  }
}



extern bool Prog(istream &in, int &line) {

  bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
    
    
    
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
            f1 = StmtList(in, line); 
			if(f1) {
				tok = Parser::GetNextToken(in, line);
				if( tok == END){
					tok = Parser::GetNextToken(in, line);
					if(tok != PROGRAM){
						ParseError(line, "Missing PROGRAM Keyword.");
						return false;
					}
					return true;
				}
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
    else if(!PROGRAM || !f1)
    {
       ParseError(line, "Incorrect Program Body");
       return false; 
    }
    else if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
        }
    else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
        ParseError(line, "Empty File");
        return true;
        }
 
    return f1;
    
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
extern bool StmtList(istream &in, int &line) {
  bool status;

  LexItem t = Parser::GetNextToken(in, line);
  if (t == END) {
    return true;
  }

  Parser::PushBackToken(t);
  status = Stmt(in, line);
  t = Parser::GetNextToken(in, line);
  // cout << t.GetToken() << ", " << t.GetLexeme() << ", " << t.GetLinenum() <<
  // endl;
  if (t == PROGRAM && !status) {
    ParseError(line, "Syntactic error in Program Body.");
    return false;
  } else {
    Parser::PushBackToken(t);
  }
  return status;
}//End of StmtList function

//Stmt ::= DeclStmt | ControlStmt
extern bool Stmt(istream &in, int &line) {
  bool status=false;
	
	LexItem t = Parser::GetNextToken(in, line);
    
  
    
    if(t == END){
        return true;
        t = Parser::GetNextToken(in, line);
        if(t == PROGRAM){
            Parser::PushBackToken(t);
            return false;
        }
      
    }
	
	switch( t.GetToken() ) {
	case INT: case FLOAT: case BOOL:
		Parser::PushBackToken(t);
		status = DeclStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Declaration Statement.");
			return status;
		}
		break;
	case IF: case PRINT: case IDENT: 
		Parser::PushBackToken(t);
		status = ControlStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect control Statement.");
			return status;
		}
		break;
	default:
		Parser::PushBackToken(t);
        return true;
	}
    
    LexItem tok = Parser::GetNextToken(in,line);
    if(tok != SEMICOL){
        ParseError(line, "Missing semicolon at end of Statement.");
        return false;
        
    }
    else
        status = Stmt(in, line);
    
  
	return status;
}//End of Stmt function

//DeclStmt ::= ( INT | FLOAT | BOOL ) VarList
extern bool DeclStmt(istream &in, int &line) {
 LexItem t;
	bool status;
	
	t = Parser::GetNextToken(in, line);
	if(t == INT || t == FLOAT || t == BOOL)
	{
		status = VarList(in, line, t);
		
		if (!status)
		{
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return status;
		}
        
        
	}
	else
	{
		ParseError(line, "Incorrect Declaration Type.");
		return false;
	}
	
	return true;
}//End of DeclStmt

//VarList:= Var {,Var}
extern bool VarList(istream &in, int &line, LexItem &type) {
 bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
            SymTable[identstr] = type.GetToken();
            
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = VarList(in, line, type);
	}
	else {
		Parser::PushBackToken(tok);
		return true;
	}
	return status;	
}//End of VarList

//ControlStmt ::= AssigStmt | IfStmt | PrintStmt 
extern bool ControlStmt(istream &in, int &line) {
 bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		break;
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
} // End of ControlStmt

//PrintStmt:= PRINT (ExpreList) 
bool PrintStmt(istream &in, int &line) {
  LexItem t;
    
    ValQue = new queue<Value>;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
        
        ParseError(line, "Missing expression list after Print");
        while(!(*ValQue).empty()){
            ValQue->pop();
        }
        delete ValQue;
        

		return false;
	}
	
    
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
    while (!(*ValQue).empty()){
        cout << (*ValQue).front();
       
        
        if( (*ValQue).front().IsErr()){
            ParseError(line, "Undefined Variable");
            return false;
        }
         ValQue->pop();
    }
    cout<< endl;
	return ex;
}//End of PrintStmt

//IfStmt:= IF (Expr) then StmtList [Else StmtList] END IF
extern bool IfStmt(istream &in, int &line) {
    bool status;
  LexItem t = Parser::GetNextToken(in, line);

  if (t != LPAREN) {
    ParseError(line, "Missing Left Parenthesis");
    return false;
  }

  Value val;
  status = Expr(in, line, val);
  if (!status) {
    ParseError(line, "Missing if statement Logic Expression");
    return false;
  }

  t = Parser::GetNextToken(in, line);
  if (t != RPAREN) {

    ParseError(line, "Missing Right Parenthesis");
    return false;
  }

  t = Parser::GetNextToken(in, line);
  if (t != THEN) {
    // ParseError(line, "Missing expression list after Then");
    return false;
  }

  if(val.IsBool() && val.GetBool()){
      status = StmtList(in, line);
      if (!status) {
        // ParseError(line, "If Statement Syntax Error");
        ParseError(line, "Missing Statement for If-Stmt Then-Part");
        return false;
      }
  }
  else if (val.IsBool())
  {
      while(t != ELSE && t != END){
        t = Parser::GetNextToken(in, line);
      }
      Parser::PushBackToken(t);
    }
  else{
    ParseError(line, "Missing Statement for If-Stmt If-Part");
    return false;
  }

  t = Parser::GetNextToken(in, line);
  if (t == ELSE && val.GetBool()) {
      while(t != END){
        t = Parser::GetNextToken(in, line);
      }
    }
  else if (t == ELSE){
     status = StmtList(in, line);
      if (!status) {
        ParseError(line, "Missing expression lidfst after Else");
        return false;
      }
    }
  else {
    Parser::PushBackToken(t);
  }
  
  t = Parser::GetNextToken(in, line);
  if(t == END){
    t = Parser::GetNextToken(in, line);
      if (t != IF) {
        ParseError(line, "Missing closing keywords of IF statement.");
        return false;
      }
  }
  return true;

	
}//End of IfStmt function

//Var:= ident
extern bool Var(istream &in, int &line, LexItem &idtok) {
   bool status;
   string identstr;
  LexItem tok = Parser::GetNextToken(in, line);
  if (tok == IDENT) {
    identstr = tok.GetLexeme();
    status = defVar.find(identstr)->second;
    if (!status) {
       ParseError(line, "Variable Redefinition");
       return false;
      }
  }
  else if(tok.GetToken() == ERR){
      ParseError(line, "Unrecognized Input Pattern");
      cout << "(" << tok.GetLexeme() << ")" << endl;
      return false;
}
    
  else {
      ParseError(line, "Invalid Variable.");
        return false;
}
 
  return status;

}//End of Var
    
//AssignStmt:= Var = Expr
extern bool AssignStmt(istream &in, int &line) {
    LexItem tok = Parser::GetNextToken(in, line);
    
  if (defVar.count(tok.GetLexeme()) != 1) {
    ParseError(line, "Undeclared Variable");
    return false;
  }
  LexItem t = Parser::GetNextToken(in, line);
  
  Value val1;
  bool status = Expr(in, line, val1);
  if( t== ASSOP){
      if(status){
        Token dataType = SymTable[tok.GetLexeme()];
        if(val1.IsErr()){
          ParseError(line, "Missing Expression in Assignment Statement");
          return false;
        }
        if(IdentList(dataType, val1)){
          TempsResults[tok.GetLexeme()] = val1;
        }else{
          ParseError(line, "Illegal Assignment Operation");
          return false;
        }
      }}
  else{
    ParseError(line, "Missing assignment operator");
    return false;
  }
  return status;
}//End of AssignStmt

//ExprList:= Expr {,Expr}
extern bool ExprList(istream &in, int &line) {
  bool status = false;
	Value retVal;
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
    ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
		
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
} // End of ExprList

//Expr ::= LogORExpr ::= LogANDExpr { || LogANDRxpr }
extern bool Expr(istream &in, int &line, Value &retVal) {
    Value val1, val2;
	LexItem tok;
	bool t1 = LogANDExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
    
    retVal = val1;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == OR ) 
	{
		t1 = LogANDExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
        
        retVal = retVal || val2;
        if(retVal.IsErr()){
            ParseError(line, "Illegal OR operation.");
            return false;
        }
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr/LogORExpr

// LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream &in, int &line, Value &retVal) {
  
    Value val1, val2;
	LexItem tok;
	bool t1 = EqualExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	
    retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == AND ) 
	{
		t1 = EqualExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
        
        retVal = retVal && val2;
        if (retVal.IsErr()){
            ParseError(line, "Illegal AND operation.");
            return false;
        }
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	
	Parser::PushBackToken(tok);
	return true;
} // End of LogANDExpr
    
//EqualExpr ::= RelExpr [ == RelExpr ]
extern bool EqualExpr(istream &in, int &line, Value &retVal) {
   Value val1, val2;
	LexItem tok;
	bool t1 = RelExpr(in, line, val1);
		
	if( !t1 ) {
        ParseError(line, "Missing operand after operator");
		return false;
	}
	
    retVal = val1;
    
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == EQUAL ) 
	{
		t1 = RelExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
        
        retVal = retVal == val2;
        if(retVal.IsErr()){
            ParseError(line, "Illegal EQUAL operation.");
            return false;
        }
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of EqualExpr
    
//RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
extern bool RelExpr(istream &in, int &line, Value &retVal) {
 
    Value val1, val2;
	LexItem tok;
	bool t1 = AddExpr(in, line, val1);
		
	if( !t1 ) {
        ParseError(line, "Missing operand after operator");
		return false;
	}
	
    retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == LTHAN || tok == GTHAN) 
	{
		t1 = AddExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
        
        retVal = (tok == GTHAN)? retVal > val2: retVal < val2;
		
        if(retVal.IsErr()){
            ParseError(line, (tok == GTHAN)? "Illegal Greather Than operation.": "Illegal Less Than operation");
            return false;
        }

		tok = Parser::GetNextToken(in, line);
		
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr
    
//AddExpr :: MultExpr { ( + | - ) MultExpr }
extern bool AddExpr(istream &in, int &line, Value &retVal) {
   Value val1, val2;
	
	bool t1 = MultExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
    retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = MultExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
       if(tok == PLUS){
            retVal = retVal + val2;
            if(retVal.IsErr()){
                ParseError(line,"Illegal Addition operation");
                return false;
            }
        }
        else if(tok == MINUS){
            retVal = retVal - val2;
            if(retVal.IsErr()){
                ParseError(line, "Illegal Subtraction operation");
                return false;
            }
        }
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr
    
//MultExpr ::= UnaryExpr { ( * | / ) UnaryExpr }
extern bool MultExpr(istream &in, int &line, Value &retVal) {
  Value val1, val2;
	
	bool t1 = UnaryExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
    retVal = val1;
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		t1 = UnaryExpr(in, line, val2);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
        if(tok == MULT){
            retVal = retVal * val2;
            if(retVal.IsErr()){
                ParseError(line,"Illegal multiplicaiton operation");
                return false;
            }
        }
        else if(tok == DIV ){
            if(val2.GetInt() ==0 || val2.GetReal() ==0){
                ParseError(line, "Run-Time Error-Illegal Division by Zero ");
                return false;
            }
        }
        
            //
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
    
	Parser::PushBackToken(tok);
    return true;
}//End of MultExpr
    
//UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
extern bool UnaryExpr(istream &in, int &line, Value &retVal) {
 
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = 1;
	}
	else if(t == PLUS)
	{
		sign = 2 ;
	}
    else if (t == NOT){
        sign = 3;
    }
	else
		Parser::PushBackToken(t);
		
	status = PrimaryExpr(in, line, sign, retVal);
   
	return status;
}//End of UnaryExpr
 
//PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | ( Expr )
extern bool PrimaryExpr(istream &in, int &line, int sign, Value &retVal) {
  LexItem tok = Parser::GetNextToken(in, line);
  if( tok == IDENT ) {
		
		if(SymTable[tok.GetLexeme()] == PROGRAM){
      ParseError(line, "Illegal use of program name as a variable");
      return false;
    }
    retVal = Value(TempsResults[tok.GetLexeme()]);
	}
	else if( tok == ICONST ) {
		
		retVal = Value(stoi(tok.GetLexeme()));
	}
	else if( tok == SCONST ) {
		
		retVal = Value(tok.GetLexeme());
	}
	else if( tok == RCONST ) {
		
		 retVal = Value(stof(tok.GetLexeme()));
	}
	else if( tok == BCONST ) {
		
		retVal = (tok.GetLexeme() == "TRUE") ? Value(true) : Value(false);
	}
	
  
  bool ex;
  if (tok == LPAREN) {
    Value val1;
    ex = Expr(in, line, val1);
    if (!ex) {
      ParseError(line, "Missing expression list after Print");
      return false;
    }
    tok = Parser::GetNextToken(in, line);
    if (tok != RPAREN) {

      ParseError(line, "Missing Right Parenthesis");
      return false;
    }
    retVal = val1;
  }
    
  if(sign  ==1){
    if(retVal.IsInt()){
      retVal = Value(-retVal.GetInt());
      return true;
    }else if(retVal.IsReal()){
      retVal = Value(-retVal.GetReal());
      return true;
    }else{
      ParseError(line, "Illegal Operand Type for Sign Operator");
      return false;
    }
  }

  else if (sign == 2){
    if(!(retVal.IsInt() || retVal.IsReal())){
      ParseError(line, "Illegal Operand Type for Sign Operator");
      return false;
    }
    return true;
}
  else if (sign == 3) {
    if(retVal.IsBool()){
      retVal = Value(!retVal);
      return true;
    }
    return false;
  }

  return true;
}
