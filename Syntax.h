#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <tuple>

using namespace std;

class NonTerminals {

public:
    string name;

    vector <string> firstSets;
    vector <string> followSets;
    
    vector <vector <string> > productionRules;

    bool match(vector <string> classPart);
    bool checkProduction(vector <string> classPart);

    string extractNT(string nt);
    NonTerminals searchNT(string nt);
};

//extern vector <NonTerminals> NT;

class Syntax {

public:

    Syntax() {};
    Syntax(vector <tuple <string, string, int> > tokenSet);

    //static vector <string> tokens;

    void createNT();
    void analyzer();
};