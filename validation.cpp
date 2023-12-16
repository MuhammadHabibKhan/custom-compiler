#include "Lexical.h"

bool Lexical::isIdentifier(string valuePart)
{
    // regex pattern("(+ [a-zA-Z]) ([a-zA-Z]+[0-9]+)");
    regex bv("[a-zA-Z + _][_ + a-zA-Z + 0-9]*");
    return regex_match(valuePart, bv);
}

bool Lexical::isInt(string valuePart)
{
    regex bv("[-+]?[0-9]+");
    return regex_match(valuePart, bv);
}

bool Lexical::isFloat(string valuePart)
{
    regex bv("[-+]?[0-9]*[.]?[0-9]+|[-+]?[0-9]+[.]?[0-9]*");
    return regex_match(valuePart, bv);
}

bool Lexical::isDigit(string valuePart)
{
    regex bv("[0-9]");
    string d(1, valuePart[0]);
    return regex_match(d, bv);
}
bool Lexical::isLetter(string valuePart)
{
    regex bv("[a-zA-Z]");
    string d(1, valuePart[0]);
    return regex_match(d, bv);
}

bool Lexical::checkInMap(unordered_map<string, string> map, string valuePart)
{
    if (map.count(valuePart) > 0)
    {
        return true;
    }
    return false;
}

bool Lexical::isTerminator(string valuePart)
{
    return checkInMap(lexemeTerminator, valuePart);
}
bool Lexical::isPunctuators(string valuePart)
{
    unordered_map<string, string> punctuator;
    punctuator = {
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
    };
    return checkInMap(punctuator, valuePart);
}
bool Lexical::isConditionalOperator(string valuePart)
{
    unordered_map<string, string> conditionalOp;
    conditionalOp = {
        {"<", "<"},
        {">", ">"},
        {"==", "=="},
        {"!=", "!="},
        {"<=", "<="},
        {">=", ">="},
    };
    return Lexical::checkInMap(conditionalOp, valuePart);
}
bool Lexical::isArithmeticOperator(string valuePart)
{
    unordered_map<string, string> arithmeticOp;
    arithmeticOp = {
        {"+", "+"},
        {"-", "-"},
        {"/", "/"},
        {"*", "*"},
        {"^", "^"},
        {"%", "%"},
    };
    return Lexical::checkInMap(arithmeticOp, valuePart);
}
bool Lexical::isPlusMinus(string valuePart)
{
    unordered_map<string, string> plusMinus;
    plusMinus = {
        {"+", "+"},
        {"-", "-"},
    };
    return checkInMap(plusMinus, valuePart);
}
bool Lexical::isMDM(string valuePart)
{
    unordered_map<string, string> MDM;
    MDM = {
        {"/", "/"},
        {"*", "*"},
        {"%", "%"},
    };
    return checkInMap(MDM, valuePart);
}
bool Lexical::isLogicalOperator(string valuePart)
{
    unordered_map<string, string> logicalOp;
    logicalOp = {
        {"NOT", "!"},
        {"AND", "&"},
        {"OR", "|"},
    };
    return checkInMap(logicalOp, valuePart);
}
bool Lexical::isAndOperator(string valuePart)
{
    unordered_map<string, string> andOp;
    andOp = {
        {"NOT", "!"},
        {"AND", "&"},
        {"OR", "|"},
    };
    return checkInMap(andOp, valuePart);
}
bool Lexical::isOrOperator(string valuePart)
{
    unordered_map<string, string> orOp;
    orOp = {
        {"NOT", "!"},
        {"AND", "&"},
        {"OR", "|"},
    };
    return checkInMap(orOp, valuePart);
}
bool Lexical::isNot(string valuePart)
{
    unordered_map<string, string> notOp;
    notOp = {
        {"!", "!"},
    };
    return checkInMap(notOp, valuePart);
}
bool Lexical::isKeywords(string valuePart)
{
    return checkInMap(keywords, valuePart);
}
bool Lexical::isAccessModifier(string valuePart)
{
    unordered_map<string, string> accessMd;
    accessMd = {
        {"private", "private"},
        {"public", "public"},
        {"protected", "protected"} };
    return checkInMap(accessMd, valuePart);
}
bool Lexical::isDt(string valuePart)
{
    unordered_map<string, string> dT;
    dT = {
        {"int", "int"},
        {"float", "float"},
        {"bool", "bool"},
        {"string", "string"},
        {"char", "char"},
    };
    return checkInMap(dT, valuePart);
}
bool Lexical::isBreakCont(string valuePart)
{
    unordered_map<string, string> breakCont;
    breakCont = {
        {"break", "break"},
        {"continue", "continue"},
    };
    return checkInMap(breakCont, valuePart);
}
bool Lexical::isBool(string valuePart)
{
    unordered_map<string, string> bol;
    bol = {
        {"true", "true"},
        {"false", "false"},
    };
    return checkInMap(bol, valuePart);
}
bool Lexical::isChar(string valuePart)
{
    // cout << "vp: " << valuePart << endl;
    if (isLetter(valuePart))
    {
        return true;
    }
    unordered_map<string, string> charac;
    charac = {
        {"\t", "\t"},
        {"\n", "\n"},
        {"\r", "\r"},
        {"\\", "\\"},
    };
    return checkInMap(charac, valuePart);
}
void Lexical::assignClassPart()
{
    for (auto& i : token)
    {
        string valuePart = get<1>(i);
        // cout << valuePart << endl;

        if (get<0>(i) != "-")
            continue;

        else if (isLetter(valuePart))
        {
            if (isKeywords(valuePart))
            {
                // syntactically replacable handling
                if (isDt(valuePart))
                {
                    get<0>(i) = "DT";
                }
                else if (isAccessModifier(valuePart))
                {
                    get<0>(i) = "access-modifier";
                }
                else if (isBreakCont(valuePart))
                {
                    get<0>(i) = valuePart;
                }
                else if (isBool(valuePart))
                {
                    get<0>(i) = "bool literal";
                }
                // other handling
                else
                {
                    get<0>(i) = valuePart;
                }
            }
            else if (isIdentifier(valuePart))
            {
                get<0>(i) = "ID";
            }
            else
            {
                get<0>(i) = "invalid";
            }
        }
        else if (isDigit(valuePart))
        {
            if (isInt(valuePart))
            {
                get<0>(i) = "int literal";
            }
            else if (isFloat(valuePart))
            {
                get<0>(i) = "float literal";
            }
            else
            {
                get<0>(i) = "Invalid";
            }
        }
        else if (isTerminator(valuePart))
        {

            if (isPunctuators(valuePart))
            {
                get<0>(i) = valuePart;
            }
            else if (isConditionalOperator(valuePart))
            {
                get<0>(i) = "ro";
            }
            else if (isArithmeticOperator(valuePart))
            {
                if (isPlusMinus(valuePart))
                {
                    get<0>(i) = "pm";
                }
                else if (isMDM(valuePart))
                {
                    get<0>(i) = "mdm";
                }
            }
            else if (isLogicalOperator(valuePart))
            {
                if (isAndOperator(valuePart))
                {
                    get<0>(i) = "and";
                }
                else if (isOrOperator(valuePart))
                {
                    get<0>(i) = "or";
                }
                else if (isNot(valuePart))
                {
                    get<0>(i) = "not";
                }
            }
            else
            {
                get<0>(i) = valuePart;
            }
        }
        else if (isChar(valuePart))
        {
            get<0>(i) = "char literal";
        }
        else if (isFloat(valuePart))
        {
            get<0>(i) = "float literal";
        }
        else
        {
            get<0>(i) = "Invalid";
        }
    }
}

void Lexical::writeTokens()
{
    std::ofstream tokenFile("tokens.txt");

    if (!tokenFile.is_open())
    {
        std::cout << "Could not open the output file." << std::endl;
        return;
    }

    for (auto& i : token)
    {
        tokenFile << get<0>(i) << " | " << get<1>(i) << " | " << get<2>(i) << endl;
    }

    tokenFile.close();
}