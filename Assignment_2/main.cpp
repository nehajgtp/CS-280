#include <iostream>
#include <fstream>
using std::ifstream;
#include <string>
#include <iterator>
#include <map>
#include <vector>
#include <algorithm>
#include "lex.h"

using namespace std;

int main(int argc, char *argv[])
{

    int lineNumber = 0;
    istream *in = &cin;
    ifstream file;
    bool vflag = false;
    bool idsflag = false;
    bool constsflag = false;
    bool constslabel_str = false;
    bool constslabel_int = false;
    bool onefile = false;
    bool cannotopen = false;
    bool unrecognizedflag = false;
    int tokCount = 0;
    map<string, bool> ids;
    vector<string> strings;
    vector<string> integers;

    for (int i = 1; i < argc; i++)
    {
        string arg(argv[i]);
        if (arg == "-v")
            vflag = true;
        else if (arg == "-ids")
            idsflag = true;
        else if (arg == "-consts")
            constsflag = true;
        else if (arg[0] == '-')
        {
            cout << "UNRECOGNIZED FLAG " << arg << endl;
            unrecognizedflag = true;
        }
        else if (in != &cin)
        {
            cout << "ONLY ONE FILE NAME ALLOWED" << endl;
            onefile = true;
        }
        else
        {
            file.open(arg);
            if (file.is_open() == false)
            {
                cout << "CANNOT OPEN " << arg << endl;
                cannotopen = true;
            }

            in = &file;
        }
    }

    if (onefile == false && cannotopen == false && unrecognizedflag == false)
    {
        Lex l;
        string iden = "IDENTIFIERS: ";
        int iden_count = 0;

        while (((l = getNextToken(*in, lineNumber)).GetToken()) != DONE && l != ERR)
        {

            if (vflag)
            {
                cout << l << endl;
            }
            tokCount++;
            if (idsflag)
            {
                if (l.GetToken() == ID)
                {
                    ids[l.GetLexeme()] = true;
                    iden_count++;
                }
            }

            if (constsflag)
            {
                if (l.GetToken() == STR)
                {
                    constslabel_str = true;
                    strings.push_back(l.GetLexeme());
                }

                if (l.GetToken() == INT)
                {
                    constslabel_int = true;
                    integers.push_back(l.GetLexeme());
                }
            }
        }

        sort(integers.begin(), integers.end(), greater<string>());
        sort(strings.begin(), strings.end());

        if (idsflag == true && iden_count > 0)
        {
            cout << iden;
            auto it = ids.begin();
            cout << it->first;

            for (it++; it != ids.end(); it++)
                cout << ", " << it->first;
            cout << endl;
        }

        if (l.GetToken() == ERR)
        {
            cout << "Error on line " << (l.GetLinenum() + 1) << " (" << l.GetLexeme() << ")" << endl;
        }

        if (constslabel_str && l.GetToken() == DONE)
        {
            cout << "STRINGS:" << endl;
            for (string x : strings)
                cout << x << endl;
        }

        if (constslabel_int && l.GetToken() == DONE)
        {
            cout << "INTEGERS:" << endl;
            for (auto y : integers)
                cout << y << endl;
        }

        if (l.GetToken() == DONE)
        {
            cout << "Lines: " << l.GetLinenum() << endl;
            if (getNextToken(*in, lineNumber).GetLinenum() > 0)
            {
                cout << "Tokens: " << tokCount << endl;
            }
        }
    }
}
