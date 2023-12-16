#ifndef SEMANTICS_H
#define SEMANTICS_H
#pragma once

#include <iostream>
#include <stack>
#include <tuple>
#include <vector>

using namespace std;

class Semantics {

public:

	int scopeID;
	int currentScope;
	stack <int> scopeStack;

	bool declareFlag;
	bool undeclareFlag;

	// vector of ID names as multiple declaration using commas
	tuple <vector <string> , string, int, string> checkDeclare;

	// name, type, scope, value
	tuple <string, string, int, string> undeclareCheck;

	tuple <string, string, string, string> currentClass;

	// scope Table -> name, type, scope
	vector < tuple <string, string, int> > scopeTable;

	// member table -> name, type, access modifier, type modifier
	vector < tuple <string, string, string, string> > memberTable;

	// definition table -> name, type, access modifier, parent, type modifier, member table reference
	vector < tuple <string, string, string, string, string, vector < tuple <string, string, string, string> > > > definitionTable;

	void createScope(string classPart);
	void destroyScope(string classPart);

	bool insertST(string name, string type, int scope, string rule);
	bool insertMT(string name, string type, string access_modifier, string type_modifier);
	bool insertDT(string name, string type, string access_modifier, string parent, string type_modifier, vector < tuple <string, string, string, string> >);

	void flagHandler(string rule);
	void deFlagger(string rule);

	void createDeclare(string cp, string vp);
	void handleDeclare(string rule);
	bool declareTypeCheck(string DT, string valueCP);

	void createUndeclare(string cp, string vp);

	string lookupST(string name, int scope);

};

#endif // !SEMANTICS_H
