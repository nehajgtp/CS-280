#include <iostream>
#include <fstream>
#include <iterator>
#include <cctype>
#include <string>
#include <map>
#include "lex.h"

using namespace std;

std::map<Token, string> print_token = {
    {PRINT, "PRINT"}, {LET, "LET"}, {IF, "IF"}, {LOOP, "LOOP"}, {BEGIN, "BEGIN"}, {END, "END"}, {ID, "ID"}, {INT, "INT"}, {STR, "STR"}, {PLUS, "PLUS"}, {MINUS, "MINUS"}, {STAR, "STAR"}, {SLASH, "SLASH"}, {BANG, "BANG"}, {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, {SC, "SC"}, {ERR, "ERR"}, {DONE, "DONE"}

};

ostream &operator<<(ostream &out, const Lex &tok)
{
    Token tt = tok.GetToken();

    string lm = tok.GetLexeme();

    if (tt == ID || tt == INT || tt == STR)
    {
        out << print_token[tt] << "(" << lm << ")";
    }

    if (tt == PLUS || tt == MINUS || tt == BANG || tt == STAR || tt == SLASH || tt == SC || tt == IF || tt == PRINT || tt == LOOP || tt == BEGIN || tt == END || tt == LET)
    {
        out << print_token[tt];
    }
    return out;
}

static map<string, Token> kwmap = {{"if", IF}, {"print", PRINT}, {"loop", LOOP}, {"begin", BEGIN}, {"end", END}, {"let", LET}};

Lex idkw(string lexeme, int linenum)
{
    Token tt = ID;
    auto kIt = kwmap.find(lexeme);
    if (kIt != kwmap.end())
    {
        tt = kIt->second;
    }

    return Lex(tt, lexeme, linenum);
}

Lex getNextToken(istream &in, int &linenum)
{
    enum LexState
    {
        BEGIN,
        IN_IDENTIFIER,
        POSSIBLE_NEGATIVE,
        IN_INTEGER,
        IN_STRING,
        SAW_BACKSLASH,
        IN_COMMENT
    } lexstate = BEGIN;
    string lexeme;
    char c;

    while (in.get(c))
    {

        if (c == '\n')
        {
            linenum++;
        }

        switch (lexstate)
        {
        case BEGIN:
            if (isspace(c))
            {
                continue;
            }

            else if (isdigit(c))
            {
                lexeme += c;
                lexstate = IN_INTEGER;
            }
            else if (c == '/')
            {
                if (in.peek() == '/')
                {
                    lexstate = IN_COMMENT;
                }

                else
                {
                    return Lex(SLASH, "/", linenum);
                }
            }

            else if (c == '"')
            {
                lexeme += c;
                lexstate = IN_STRING;
            }
            else if (isalpha(c))
            {
                lexeme += c;
                lexstate = IN_IDENTIFIER;
            }

            else if (c == '-')
            {
                lexstate = POSSIBLE_NEGATIVE;
            }

            else if (c == '+')
            {
                return Lex(PLUS, "+", linenum);
            }

            else if (c == '*')
            {
                return Lex(STAR, "*", linenum);
            }

            else if (c == '!')
            {
                return Lex(BANG, "!", linenum);
            }

            else if (c == '(')
            {
                return Lex(LPAREN, "(", linenum);
            }

            else if (c == ')')
            {
                return Lex(RPAREN, ")", linenum);
            }

            else if (c == ';')
            {
                return Lex(SC, ";", linenum);
            }

            else if (in.eof())
            {
                return Lex(DONE, "DONE", linenum);
            }

            else
            {
                lexeme += c;
                return Lex(ERR, lexeme, linenum);
            }
            break;

        case IN_IDENTIFIER:
            if (isalnum(c))
            {
                lexeme += c;
            }

            else
            {
                if (c == '\n')
                {
                    linenum--;
                }
                in.putback(c);
                return idkw(lexeme, linenum);
            }
            break;
        case POSSIBLE_NEGATIVE:
            if (isdigit(in.peek()))
            {
                lexstate = IN_INTEGER;
            }

            else
            {
                in.putback(c);
                if (c == '\n')
                {
                    linenum--;
                }
                return Lex(MINUS, "-", linenum);
            }
            break;
        case IN_STRING:
            if (c == '\\')
            {
                lexstate = SAW_BACKSLASH;
            }

            else if (in.eof() || (c == '\n'))
            {
                lexeme += c;
                linenum--;
                return Lex(ERR, lexeme, linenum);
            }

            else if (c == '"')
            {
                lexeme = lexeme.substr(1, lexeme.length() - 1);
                return Lex(STR, lexeme, linenum);
            }

            else
            {
                lexeme += c;
                continue;
            }

            break;

        case SAW_BACKSLASH:
            if (c == 'n')
            {
                lexeme += '\n';
                lexstate = IN_STRING;
            }

            else
            {
                lexeme += c;
                lexstate = IN_STRING;
            }

            break;

        case IN_INTEGER:
            if (isdigit(c))
            {
                lexeme += c;
            }

            else
            {
                in.putback(c);
                if (c == '\n')
                {
                    linenum--;
                }
                return Lex(INT, lexeme, linenum);
            }
            break;

        case IN_COMMENT:
            if (c == '\n')
            {
                lexstate = BEGIN;
            }

            else
            {
                continue;
            }
            break;
        }
    }

    if (in.eof())
        return Lex(DONE, "DONE", linenum);
    return Lex(ERR, lexeme, linenum);
}
