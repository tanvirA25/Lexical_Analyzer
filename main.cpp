

#include <iostream>
#include <fstream>
#include<sstream>
#include <string>
#include <map>
#include <vector>


#include<cctype>


#include "lex.h"

using namespace std;

int main(int argc, char* argv[])
{
    bool flagv = 0;
    bool flagiconsts = 0;
    bool flagrconsts = 0;
    bool flagsconsts = 0;
    bool flagident = 0;
    bool flagbconsts = 0;
    string tfilename;
    bool filename = 0;
    ifstream file;
    
    LexItem tok;
    int lineNumber = 0;
    int tokNumber = 0;
    map<string, Token> stringMap;
    map<int, Token> intMap;
    
    map<Token,string> tokenPrint = {
		{PROGRAM, "PROGRAM"},
		{PRINT, "PRINT"},
		{INT, "INT"},
		{ END, "END" },
		{ FLOAT, "FLOAT"},
		{ BOOL, "BOOL" },
		
		{ ELSE, "ELSE" },
		{ IF, "IF" },
		
		{THEN, "THEN"},
		{ TRUE, "TRUE" },
		{ FALSE, "FALSE" },
		
		{ IDENT, "IDENT" },

		{ ICONST, "ICONST" },
		{ RCONST, "RCONST" },
		{ SCONST, "SCONST" },
		{ BCONST, "BCONST" },
		
		{ PLUS, "PLUS" },
		{ MINUS, "MINUS" },
		{ MULT, "MULT" },
		{ DIV, "DIV" },
		{ ASSOP, "ASSOP" },
		{ EQUAL, "EQUAL" },
		{ GTHAN, "GTHAN" },
		{ LTHAN, "LTHAN" },
		{ AND, "AND" },
		{ OR, "OR" },
		{ NOT, "NOT" },
            
		{ COMMA, "COMMA" },
		{ LPAREN, "LPAREN" },
		{ RPAREN, "RPAREN" },
		
		
		{ SEMICOL, "SEMICOL" },
		
		{ ERR, "ERR" },

		{ DONE, "DONE" },
    };
    
    if (argc > 1){
        
        tfilename = argv[1];
    }
    else {
        
        cerr << "NO SPECIFIED INPUT FILE NAME." << endl;
        return 0;
    }
    
    
    
    for (int i = 1; i < argc; i++) {
         
        string arg(argv[i]);
        if (arg[0] == '-')
        {
            if (arg == "-v")
                flagv = true;
            else if (arg == "-iconst")
                flagiconsts = 1;
            else if (arg == "-sconst")
                flagsconsts = 1;
            else if (arg == "-bconst")
                flagbconsts = 1;
            else if (arg == "-rconst")
                flagrconsts = 1;
            else
                if (arg == "-ident")
                    flagident = true;
                else
                {
                    cerr << "UNRECOGNIZED FLAG " << arg << endl;
                    return 0;
                }
        }
        else
        {
            if (filename)
            {
                cerr << "ONLY ONE FILE NAME ALLOWED." << endl;
                return 0;
            }
            else
            {
                filename = true;
                
            }
        }
    }
    if (!filename)
    {
        cerr << "NO SPECIFIED INPUT FILE NAME." << endl;
        return 0;
    }
    else
    {
        file.open(tfilename);
        if (!file.is_open())
        {
            cout << "CANNOT OPEN THE FILE " << tfilename << endl;
            return 0;
        }
    }


    while (!file.eof())
    {

        tok = getNextToken(file, lineNumber);
       
        if (tok == ERR)
        {            
            cout << "Error in line " << tok.GetLinenum() + 1 << " (" << tok.GetLexeme() << ')' << endl;
            return 0;
        }
        if (tok == DONE)
            break;
        stringMap.insert(pair<string, Token>(tok.GetLexeme(), tok.GetToken()));
        tokNumber++;
        if (tok == ICONST)
            intMap.insert(pair<int, Token>(stoi(tok.GetLexeme()), tok.GetToken()));

        if (flagv == 1)
        {
            if((tok == RCONST && (flagrconsts|| flagident)) || (tok == BCONST) || (tok == ICONST) || tok == IDENT)
            {
                cout << tokenPrint[tok.GetToken()] << "(" << tok.GetLexeme() << ')' << endl;
            }
            else if(tok == SCONST && flagsconsts)
            {
                cout << tokenPrint[tok.GetToken()] << "(" << (tok.GetLexeme()).substr(1, (tok.GetLexeme()).length() - 2) << ")" << endl;
            }
            else
            {
                cout << tokenPrint[tok.GetToken()] << endl;
            }
        }
       
    }
    file.close();
    if (flagv == 1)
    {
        file.open(tfilename);
        lineNumber = 0;
       
        while (!file.eof())
        {
            tok = getNextToken(file, lineNumber);
            if (tok == ERR)
            {
                
                cout << "Error in line " << tok.GetLinenum() + 1 << " (" << tok.GetLexeme() << ')' << endl;
                return 0;
            }
            if(tok==DONE)
                break;
        }
        file.close();
    }
    if (lineNumber == 0)
    {
        cout << "Lines: 0" << endl;
        return 0;
    }

    cout << "Lines: " << lineNumber << endl;
    cout << "Tokens: " << tokNumber << endl;
    map<string, Token>::iterator it;
    if (flagsconsts)
    {
        cout << "STRINGS:" << endl;
        for (it = stringMap.begin(); it != stringMap.end(); it++)
        {
            string first = it->first;
            Token second = it->second;
            if (second == SCONST)
                cout << '\"' << first.substr(1, first.length() - 2) << '\"' << endl;
        }
    }


    if (flagiconsts == 1)
    {
        map<int, Token>::iterator it;
        cout << "INTEGERS:" << endl;
        for (it = intMap.begin(); it != intMap.end(); it++)
        {
            int first = it->first;
            Token second = it->second;
            if (second == ICONST)
                cout << first << endl;
        }
    }

    if (flagrconsts ==1)
    {
        cout << "REALS:" << endl;
        for (it = stringMap.begin(); it != stringMap.end(); it++)
        {
            string first = it->first;
            Token second = it->second;
            if (second == RCONST)
                cout << first << endl;
        }
    }

    if (flagbconsts == 1)
    {
        map<string, Token>::iterator it;
        cout << "Booleans:" << endl;
        for (it = stringMap.begin(); it != stringMap.end();it++)
        {
            string first = it->first;
            Token second = it->second;
            if (second == BCONST)
                cout << first << endl;
        }
    }
    if (flagident == 1)
    {
        cout << "IDENTIFIERS:" << endl;
        for (it = stringMap.begin(); it != stringMap.end(); it++)
        {
            string first = it->first;
            Token second = it->second;
            it++;
            if (second == IDENT && it != stringMap.end())
                cout << first << ", ";
            else if (second == IDENT)
                cout << first;
            it--;

        }
        cout << endl;

    }


}







