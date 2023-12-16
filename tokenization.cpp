#include "Lexical.h"

using namespace std;

Lexical::Lexical()
{
    lexemeTerminator = {
        // space, ', ", \t, \r, \n
        {" ", " "},
        {"'", "'"},
        {"\"", "\""},
        {"\\t", "\\t"},
        {"\\r", "\\r"},
        {"\\n", "\\n"},
        // punctuators
        {"{", "{"},
        {"}", "}"},
        {"(", "("},
        {")", ")"},
        {";", ";"},
        {"[", "["},
        {"]", "]"},
        {",", ","},
        {":", ":"},
        {".", "."},
        {"?", "?"},
        // conditional operators
        {"<", "<"},
        {">", ">"},
        {"==", "=="},
        {"!=", "!="},
        {"<=", "<="},
        {">=", ">="},
        // assignment and compound assignment operators
        {"=", "="},
        // arithmetic operators
        {"+", "+"},
        {"-", "-"},
        {"/", "/"},
        {"*", "*"},
        {"^", "^"},
        {"%", "%"},
        // logical operators
        {"!", "!"},
        {"&", "&"},
        {"|", "|"},
        // comments
        {"/#", "/#"},
        {"#/", "#/"},
        {"#", "#"} };

    keywords = {
        {"while", "while"},
        {"for", "for"},
        {"break", "break"},
        {"continue", "continue"},
        {"if", "if"},
        {"elif", "elif"},
        {"else", "else"},
        {"def", "def"},
        {"main", "main"},
        {"return", "return"},
        {"void", "void"},
        {"int", "int"},
        {"float", "float"},
        {"string", "string"},
        {"char", "char"},
        {"bool", "bool"},
        {"class", "class"},
        {"from", "from"},
        {"public", "public"},
        {"private", "private"},
        {"protected", "protected"},
        {"try", "try"},
        {"catch", "catch"},
        {"finally", "finally"},
        {"const", "const"},
        {"interface", "interface"},
        {"this", "this"},
        {"super", "super"},
        {"operator", "operator"},
        {"true", "true"},
        {"false", "false"},
        {"list", "list"},
        {"set", "set"},
        {"static", "static"},
        {"and","and"},
        {"or","or"},
    };
}

void Lexical::createToken()
{
    ifstream code("input.txt");

    while (getline(code, tempLine))
    {
        cout << "Line: " << tempLine << endl;

        for (int i = 0; i < tempLine.length(); i++)
        {
            string tempString(1, tempLine[i]);
            // cout << "tempString: " << tempString << endl;

            handleSplit(tempString, tempLine, i);
        }
        // handles the last word if no breaker at the end | multiline comment not broken
        if (tempToken != "" && multiLineFlag == false)
        {
            if (stringFlag == true)
            {
                token.push_back(make_tuple("Invalid String literal", tempToken, lineNumber));
                stringFlag = false;
            }
            else if (charFlag == true)
            {
                token.push_back(make_tuple("Invalid char Literal", tempToken, lineNumber));
                charFlag = false;
            }
            else
                token.push_back(make_tuple("-", tempToken, lineNumber));

            tempToken = "";
        }
        singleLineFlag = false;
        lineNumber++;
    }
    code.close();
}

void Lexical::printTokens()
{
    for (auto& i : token)
    {
        cout << "Class Part: " << get<0>(i) << " | Value Part: " << get<1>(i) << " | Line: " << get<2>(i) << endl;
    }
}

bool Lexical::checkTerminator(string c, string line, int index)
{
    // cout << "check term: " << c << endl;
    if (lexemeTerminator.count(c) > 0)
    {
        // cout << "Found" << endl;
        return true;
    }
    return false;
}

void Lexical::handleSplit(string c, string line, int index)
{
    if (c == "/" || multiLineFlag == true)
    {
        if (stringFlag == false && singleLineFlag == false && charFlag == false)
            handleFwdSlash(c, line, index);
        else
        {
            tempToken.append(c);
        }
    }
    else if (c == ".")
    {
        if (stringFlag == false && charFlag == false && multiLineFlag == false && singleLineFlag == false)
            handleDot(c, line, index);
        else
        {
            tempToken.append(c);
        }
    }
    else if (c == "#" || singleLineFlag == true)
    {
        if (stringFlag == false && charFlag == false && multiLineFlag == false)
            handleHash(c, line, index);
        else
        {
            tempToken.append(c);
        }
    }
    else if (c == "\"" || stringFlag == true)
    {
        if (multiLineFlag == false && singleLineFlag == false && charFlag == false)
            handleString(c, line, index);
        else
        {
            tempToken.append(c);
        }
    }
    else if (c == "'" || charFlag == true)
    {
        if (multiLineFlag == false && singleLineFlag == false && stringFlag == false)
            handleChar(c, line, index);
        else
        {
            tempToken.append(c);
        }
    }
    /*else if (c == "<" || c == ">" || c == "=" || c == "!")
    {
        handleConditionalOP(c, line, index);
    }*/
    else if (c == "=")
    {
        handleEquals(c, line, index);
    }
    else
    {
        handleDefault(c, line, index);
        return;
    }
}

void Lexical::handleEquals(string c, string line, int index)
{
    string prevChar;

    //cout << "handle Equals" << endl;

    if (index > 0) 
    {
        prevChar = line[index - 1];

        if (prevChar == "=")
        {
            char doublePrevChar = line [index - 2];
            
            if (doublePrevChar == '<' || doublePrevChar == '>' || doublePrevChar == '!' || doublePrevChar == '=')
            {
                tempToken.append(c);
                return;
            }
            else
            {
                if (prevChar == "<" || prevChar == ">" || prevChar == "!" || prevChar == "=")
                {
                    token.pop_back();
                    tempToken.append(prevChar + c);

                    token.push_back(make_tuple("-", tempToken, lineNumber));
                    tempToken = "";
                }
                else
                {
                    tempToken.append(c);
                }
            }
        }

        if (prevChar == "<" || prevChar == ">" || prevChar == "!" || prevChar == "=")
        {
            token.pop_back();
            tempToken.append(prevChar + c);

            token.push_back(make_tuple("-", tempToken, lineNumber));
            tempToken = "";
        }
        else
        {
            tempToken.append(c);
        }
    }
    else
    {
        tempToken.append(c);
    }
}

//void Lexical::handleConditionalOP(string c, string line, int index)
//{
//    string nextChar;
//
//    if (index + 1 < line.length()) {
//        nextChar = line[index + 1];
//
//        if (nextChar == "=")
//        {
//            //tempToken.append(c + nextChar);
//            cout << "xx" << endl;
//            return;
//        }
//        else {
//            tempToken.append(c);
//        }
//    }
//    else
//    {
//        tempToken.append(c);
//    }
//}

void Lexical::handleFwdSlash(string c, string line, int index)
{
    string nextChar(1, line[index + 1]);
    string prevCharChar(1, line[index - 1]);

    // initial check
    if (c == "/" && multiLineFlag == false)
    {
        if (nextChar == "#")
        {
            cout << "MultiLine comment started" << endl;
            multiLineFlag = true;
            return;
        }
        else
        {
            // handleDefault(prevCharChar, line, index-1);
            handleDefault(c, line, index);
            return;
        }
    }
    // second check
    if (multiLineFlag == true)
    {
        if (tempToken == "#")
        {
            if (c == "/")
            {
                multiLineFlag = false;
                cout << "MultiLine comment ended" << endl;
                tempToken = "";
                return;
            }
            else
            {
                tempToken = "";
                return;
            }
        }
        if (c == "#")
        {
            tempToken = "#";
            return;
        }
        else
        {
            return;
        }
    }
}

void Lexical::handleDefault(string tempString, string tempLine, int i)
{
    if (!checkTerminator(tempString, tempLine, i))
    {
        tempToken.append(tempString);
    }
    else
    {
        if (tempToken != "")
        {
            token.push_back(make_tuple("-", tempToken, lineNumber));
            tempToken = "";
        }
        // works on the last char which is not appended yet
        if (tempString != " ")
        {
            token.push_back(make_tuple("-", tempString, lineNumber));
            tempToken = "";
        }
    }
}

void Lexical::handleString(string c, string line, int index)
{
    string doublePrevChar;
    string prevChar;
    string nextChar;

    if (index >= 2) {
        doublePrevChar = line[index - 2];
    }
    if (index > 0) {
        prevChar = line[index - 1];
    }
    if (index + 1 < line.length()) {
        nextChar = line[index + 1];
    }

    // initial case
    if (c == "\"" && stringFlag == false)
    {
        if (tempToken != "")
        {
            token.push_back(make_tuple("-", tempToken, lineNumber));
            tempToken = "";
        }
        stringFlag = true;
        return;
    }

    // handling escape sequences
    else if (stringFlag == true && c == "\\")
    {
        if (prevChar == "\\")
        {
            tempToken.append(c);
            return;
        }
        if (nextChar == "t")
            tempToken.append("    ");

        if (nextChar == "n")
            tempToken.append("\n");

        if (nextChar == "r")
            tempToken.append("\r");

        if (nextChar == "\\")
            return;

        return;
    }
    // handling end of string
    else if (stringFlag == true && c == "\"")
    {
        token.push_back(make_tuple("string literal", tempToken, lineNumber));
        tempToken = "";
        stringFlag = false;
    }

    else if (stringFlag == true)
    {
        if (prevChar != "\\")
        {
            tempToken.append(c);
            return;
        }

        if (doublePrevChar == "\\" && prevChar == "\\")
        {
            tempToken.append(c);
            return;
        }
    }
}

void Lexical::handleHash(string c, string line, int index)
{
    if (c == "#")
    {
        cout << "Single Line comment" << endl;
        singleLineFlag = true;
        return;
    }
    if (singleLineFlag = true)
    {
        return;
    }
}

void Lexical::handleDot(string c, string line, int index)
{
    string nextChar(1, line[index + 1]);
    if (isInt(tempToken))
    {
        tempToken.append(c);
        if (!isInt(nextChar))
        {
            token.push_back(make_tuple("-", tempToken, lineNumber));
            tempToken = "";
        }
    }
    else
    {
        if (tempToken == "" && isLetter(nextChar)) {
            token.push_back(make_tuple("-", c, lineNumber));
            return;
        }

        else if (tempToken == "")
        {
            tempToken.append(c);
            return;
        }

        token.push_back(make_tuple("-", tempToken, lineNumber));
        tempToken = "";
        tempToken.append(c);

        if (!isLetter(nextChar))
            return;
        token.push_back(make_tuple("-", tempToken, lineNumber));
        tempToken = "";
    }
}

void Lexical::handleChar(string c, string line, int index)
{
    if (tempToken.length() == 0 && c == "'")
    {
        cout << "Starting Quote" << endl;
        tempToken.append(c);
        charFlag = true;
        return;
    }
    else if (tempToken.length() == 1 && c == "'" && tempToken[0] == '\'')
    {
        cout << "Ending Quote" << endl;
        charFlag = false;
        tempToken.append(c);
        tempToken.pop_back();
        tempToken.erase(tempToken.begin());
        token.push_back(make_tuple("char literal", tempToken, lineNumber));
        tempToken = "";
        return;
    }
    else if (tempToken.length() == 3 && c == "'" && tempToken[1] == '\\' && tempToken[2] == '\\')
    {
        cout << "Ending Quote" << endl;
        charFlag = false;
        tempToken.append(c);
        tempToken.pop_back();
        tempToken.erase(tempToken.begin());
        token.push_back(make_tuple("char literal", tempToken, lineNumber));
        tempToken = "";
        return;
    }
    else if (tempToken.length() > 0 && c == "'" && tempToken[tempToken.length() - 1] != '\\')
    {
        cout << "Ending Quote" << endl;
        charFlag = false;
        tempToken.append(c);
        if (tempToken.length() > 4)
        {
            token.push_back(make_tuple("Invalid Char Literal", tempToken, lineNumber));
            tempToken = "";
        }
        else
        {
            if (tempToken.length() == 4)
            {
                if (tempToken[1] != '\\')
                {
                    cout << "token == 4" << endl;
                    token.push_back(make_tuple("Invalid Char Literal", tempToken, lineNumber));
                    tempToken = "";
                }
                else
                {
                    tempToken.pop_back();
                    tempToken.erase(tempToken.begin());
                    token.push_back(make_tuple("char literal", tempToken, lineNumber));
                    tempToken = "";
                }
            }
            else if (tempToken.length() == 3)
            {
                if (tempToken[1] == '\\')
                {
                    cout << "token == 3" << endl;
                    token.push_back(make_tuple("Invalid Char Literal", tempToken, lineNumber));
                    tempToken = "";
                }
                else
                {
                    tempToken.pop_back();
                    tempToken.erase(tempToken.begin());
                    token.push_back(make_tuple("char literal", tempToken, lineNumber));
                    tempToken = "";
                }
            }
        }
        return;
    }
    tempToken.append(c);
}
