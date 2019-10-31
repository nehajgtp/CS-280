#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>
#include <ctype.h>

using namespace std;

char getChar(char c) //returns rot13 cipher char
{
    char n = c;
    if (n >= 65 && n <= 90)
    {
        n = n - 65;
        n = n + 13;
        n = n % 26;
        n = n + 65;
    }

    else if (n >= 97 && n <= 122)
    {
        n = n - 97;
        n = n + 13;
        n = n % 26;
        n = n + 97;
    }

    return n;
}

//returns rot13 string for regular word
string getStr(string s)
{
    string n;
    for (int i = 0; i < s.length(); i++)
    {
        n += getChar(s[i]);
    }
    return n;
}

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        cout << "MISSING COMMAND\n";
    }

    else if (argc > 1)
    {
        string cl;
        cl = string(argv[1]);

        if (cl == "-r")
        {
            //if -r has second argument
            if (argc == 3)
            {
                ifstream file;
                file.open(argv[2]);

                if (file.is_open() == false)
                {
                    std::cout << argv[2] << " FILE COULD NOT BE OPENED" << endl;
                }

                else
                {
                    string line;
                    while (getline(file, line))
                    {
                        string converted = getStr(line);
                        std::cout << converted << endl;
                    }

                    file.close();
                }
            }
            // if -r has only one argument
            if (argc == 2)
            {
                string line;
                while (getline(cin, line))
                {
                    string converted = getStr(line);
                    cout << converted << endl;
                }
            }
        }

        else if (cl == "-g")
        {

            char alphabet1[26];
            char letter1 = 97;
            for (int k = 0; k < 26; k++)
            {
                alphabet1[k] = letter1;
                letter1++;
            }

            char alphabet[26];
            char letter = 97;
            for (int i = 0; i < 26; i++)
            {
                alphabet[i] = letter;
                letter++;
            }

            int alpha_set = 26;
            char scrambled[26];

            for (int i = 0; i < 26; i++)
            {
                int val = rand() % alpha_set;
                scrambled[i] = alphabet[val];
                alphabet[val] = alphabet[alpha_set - 1];
                alpha_set = alpha_set - 1;
            }

            for (int j = 0; j < 26; j++)
            {
                if (alphabet1[j] == scrambled[j])
                {
                    char temp = scrambled[j];
                    scrambled[j] = scrambled[j + 1];
                    scrambled[j + 1] = temp;
                }
                std::cout << alphabet1[j] << scrambled[j] << std::endl;
            }
        }
        else if (cl == "-e" || cl == "-d")
        {
            if (argc == 2)
            {
                cout << "NO DICTIONARY GIVEN" << endl;
            }

            else
            {
                ifstream inFile;
                if (argc == 3)
                {

                    inFile.open(argv[2]);

                    if (inFile.is_open() == false)
                    {
                        std::cout << argv[2] << " DICTIONARY COULD NOT BE OPENED" << endl;
                    }

                    else
                    {
                        string pair;
                        array<char, 26> second_letters;
                        int second_letters_count = 0;
                        int letter_count = 97;
                        bool missingletter = false;

                        while (getline(inFile, pair))
                        {

                            if ((char)letter_count < pair[0])
                            {

                                cout << "MISSING LETTER " << (char)letter_count << endl;
                                missingletter = true;
                                break;
                            }

                            second_letters[second_letters_count] = pair[1];
                            second_letters_count++;
                            letter_count++;
                        }

                        if (missingletter == false)
                        {
                            for (int a = 0; a < second_letters.size(); a++)
                            {
                                for (int b = a + 1; b < second_letters.size(); b++)
                                {
                                    if (second_letters[a] == second_letters[b])
                                    {
                                        cout << "DUPLICATE CIPHERTEXT " << second_letters[a] << endl;

                                        break;
                                    }
                                }
                            }
                        }
                    }

                    inFile.close();
                }
            }

            if (argc == 4 && string(argv[1]) == "-e")
            {
                ifstream poem;
                ifstream cipher;
                cipher.open(argv[2]);
                poem.open(argv[3]);

                if (poem.is_open() == false)
                {
                    std::cout << argv[3] << " FILE COULD NOT BE OPENED" << endl;
                }

                else
                {
                    string line;
                    char arr[26];
                    int y = 0;
                    while (getline(cipher, line))
                    {
                        arr[y] = line[1];
                        y++;
                    }

                    string line2;
                    string decrypted;
                    while (getline(poem, line2))
                    {
                        string encrypted;
                        for (int i = 0; i < line2.length(); i++)
                        {
                            char ascii = line2[i];

                            if (ascii >= 65 && ascii <= 90)
                            {
                                // convert to lowercase & get value from arr & revert to uppercase & add char to encrypted
                                char newChar = tolower(ascii);
                                int value = (int)newChar - 97;
                                char lowercase_char = arr[value];
                                char encrypt = toupper(lowercase_char);
                                encrypted += encrypt;
                            }

                            else if (ascii >= 97 && ascii <= 122)
                            {
                                //get value from arr and add char to encrypted
                                int value1 = (int)ascii - 97;
                                encrypted += arr[value1];
                            }

                            else
                            {
                                //get char of ascii value and add to encrypted
                                encrypted += ascii;
                            }
                        }

                        cout << encrypted << endl;
                    }
                }
                poem.close();
                cipher.close();
            }

            if (argc == 4 && cl == "-d")
            {
                string line;
                ifstream cipher;
                ifstream poem;
                poem.open(argv[3]);
                cipher.open(argv[2]);

                if (poem.is_open() == false)
                {
                    std::cout << argv[3] << " FILE COULD NOT BE OPENED" << endl;
                }

                else
                {
                    char arr_normal[26];
                    char arr_cipher[26];
                    int y = 0;

                    while (getline(cipher, line))
                    {
                        arr_normal[y] = line[0];
                        arr_cipher[y] = line[1];
                        y++;
                    }

                    string line2;
                    string decrypted;
                    while (getline(poem, line2))
                    {
                        string encrypted;

                        for (int i = 0; i < line2.length(); i++)
                        {
                            char ascii = line2[i];

                            if (ascii >= 65 && ascii <= 90)
                            {
                                char newChar = tolower(ascii);
                                int value;
                                for (int j = 0; j < 26; j++)
                                {
                                    if (newChar == arr_cipher[j])
                                    {
                                        value = j;
                                    }
                                }

                                char lowercase_char = arr_normal[value];
                                char encrypt = toupper(lowercase_char);
                                encrypted += encrypt;
                            }

                            else if (ascii >= 97 && ascii <= 122)
                            {
                                int v;
                                for (int k = 0; k < 26; k++)
                                {
                                    if (ascii == arr_cipher[k])
                                    {
                                        v = k;
                                    }
                                }

                                char lowercase = arr_normal[v];
                                encrypted += lowercase;
                            }

                            else
                            {
                                //get char of ascii value and add to encrypted
                                encrypted += ascii;
                            }
                        }

                        cout << encrypted << endl;
                    }
                    cipher.close();
                    poem.close();
                }
            }

            if (argc == 3 && cl == "-d")
            {
                string line;
                ifstream cipher;
                cipher.open(argv[2]);
                char arr_normal[26];
                char arr_cipher[26];
                int y = 0;

                while (getline(cipher, line))
                {
                    arr_normal[y] = line[0];
                    arr_cipher[y] = line[1];
                    y++;
                }

                string txt;
                while (getline(cin, txt))
                {
                    string decrypted;
                    for (int i = 0; i < txt.length(); i++)
                    {
                        char ascii = txt[i];

                        //uppercase char cipher -> normal
                        if (ascii >= 65 && ascii <= 90)
                        {
                            char newChar = tolower(ascii);
                            int val;
                            for (int j = 0; j < 26; j++)
                            {
                                if (newChar == arr_cipher[j])
                                {
                                    val = j;
                                }
                            }

                            char lowercase_char = arr_normal[val];
                            char decrypt = toupper(lowercase_char);
                            decrypted += decrypt;
                        }

                        else if (ascii >= 97 && ascii <= 122)
                        {
                            int v;
                            for (int k = 0; k < 26; k++)
                            {
                                if (arr_cipher[k] == ascii)
                                {
                                    v = k;
                                }
                            }

                            char new_char = arr_normal[v];
                            decrypted += new_char;
                        }

                        else
                        {
                            decrypted += ascii;
                        }
                    }

                    cout << decrypted << endl;
                }
            }
        }

        else if (argc > 3)
        {
            cout << "TOO MANY ARGUMENTS" << endl;
        }

        else
        {
            cout << cl << " NOT A VALID COMMAND" << endl;
        }
    }
}
