#ifndef LEXICAL_H
#define LEXICAL_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <regex>

using namespace std;

class Lexical {

public:

    int lineNumber = 1;
    string tempToken;
    string tempLine;
    bool multiLineFlag = 0;
    bool singleLineFlag = 0;
    bool stringFlag = 0;
    bool charFlag = 0;
    int charCount = 0;

    unordered_map <string, string> lexemeTerminator;
    vector <tuple <string, string, int> > token;
    unordered_map<string, string> keywords;

    Lexical();

    //tokenization functions
    void printTokens();
    void handleFwdSlash(string c, string line, int index);
    void handleDot(string c, string line, int index);

    //void handleConditionalOP(string c, string line, int index);
    void handleEquals(string c, string line, int index);

    void handleChar(string c, string line, int index);
    void handleString(string c, string line, int index);
    void handleHash(string c, string line, int index);
    void handleDefault(string tempString, string tempLine, int i);
    void handleSplit(string c, string line, int index);
    bool checkTerminator(string c, string line, int index);
    void createToken();

    // regular expression handling
    bool isIdentifier(string valuePart);
    bool isInt(string valuePart);
    bool isFloat(string valuePart);
    bool isDigit(string valuePart);
    bool isLetter(string valuePart);

    // validation check functions
    bool checkInMap(unordered_map<string, string> map, string valuePart);
    bool isTerminator(string valuePart);
    bool isPunctuators(string valuePart);
    bool isConditionalOperator(string valuePart);
    bool isArithmeticOperator(string valuePart);
    bool isPlusMinus(string valuePart);
    bool isMDM(string valuepart);
    bool isLogicalOperator(string valuePart);
    bool isAndOperator(string valuePart);
    bool isOrOperator(string valuePart);
    bool isNot(string valuePart);
    bool isKeywords(string valuePart);
    bool isAccessModifier(string valuePart);
    bool isDt(string valuePart);
    bool isBreakCont(string valuePart);
    bool isBool(string valuePart);
    bool isChar(string valuePart);
    void assignClassPart();

    void writeTokens();

};

#endif