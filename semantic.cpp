#include "Semantic.h"

using namespace std;

void Semantics::createScope(string classPart)
{
	if (classPart == "{")
	{
		scopeStack.push(scopeID);
		scopeID++;
	}
}

void Semantics::destroyScope(string classPart)
{
	if (classPart == "}")
	{
		scopeStack.pop();
	}
}

bool Semantics::insertST(string name, string type, int scope, string rule) 
{
	if (declareFlag == true)
	{
		for (auto& entry : scopeTable)
		{
			string entryName = get<0>(entry);
			int entryScope = get<2>(entry);

			if (name == entryName && scope == entryScope)
			{
				cout << "variable " << name << " already exists" << endl;
				return false;
			}
		}

		scopeTable.push_back(make_tuple(name, type, scope));

		cout << name << " inserted into scope table" << endl;
		return true;
	}
	return false;
}

bool Semantics::insertMT(string name, string type, string access_modifier, string type_modifier)
{
	for (auto& entry : memberTable)
	{
		string entryName = get<0>(entry);

		if (name == entryName)
		{
			return false;
		}
	}
	return true;
}

bool Semantics::insertDT(string name, string type, string access_modifier, string parent, string type_modifier, vector < tuple <string, string, string, string> >)
{
	for (auto& entry : definitionTable)
	{
		string entryName = get<0>(entry);

		if (name == entryName)
		{
			return false;
		}
	}
	return true;
}

void Semantics::flagHandler(string rule)
{
	if (rule == "declare")
	{
		declareFlag = true;

		vector <string> IDs = get<0>(checkDeclare);
		IDs.clear();

		checkDeclare = make_tuple(IDs, "null", 888, "");
	}

	if (rule == "id_decl")
	{
		undeclareFlag = true;
		undeclareCheck = make_tuple("", "", 888, "");
	}

}

void Semantics::deFlagger(string rule)
{
	if (rule == "declare" && declareFlag == true)
	{
		declareFlag = false;

		vector <string> IDs = get<0>(checkDeclare);
		IDs.clear();

		checkDeclare = make_tuple(IDs, "null", 888, "");

		cout << "--------------------------" << endl;
		cout << "Declare Flag Off" << endl;
		cout << "--------------------------" << endl;
	}

	if (rule == "id_decl" &&  undeclareFlag == true)
	{
		undeclareFlag = false;
		undeclareCheck = make_tuple("", "", 888, "");
	}
}

void Semantics::createDeclare(string cp, string vp)
{
	if (declareFlag == true)
	{
		int scope = get<2>(checkDeclare);

		if (scope == 888)
		{
			scope = scopeID;
		}
		
		if (cp == "DT")
		{
			get<1>(checkDeclare) = vp;
		}

		if (cp == "ID")
		{
			get<0>(checkDeclare).push_back(vp);
		}

		if (cp == "int literal" || cp == "char literal" || cp == "string literal" || cp == "bool literal" || cp == "float literal")
		{
			get<3>(checkDeclare) = cp;
		}
	}
}

void Semantics::handleDeclare(string rule)
{
	if (declareFlag == true && rule == "declare")
	{
		int scope = get<2>(checkDeclare);
		string type = get<1>(checkDeclare);
		string valueCP = get<3>(checkDeclare);
		vector <string> IDs = get<0>(checkDeclare);

		if (!valueCP.empty())
		{
			declareTypeCheck(type, valueCP);
		}

		for (auto& ID : IDs)
		{
			insertST(ID, type, scope, rule);
		}
	}
}

bool Semantics::declareTypeCheck(string DT, string valueCP)
{
	if (DT == "int")
	{
		if (valueCP == "int literal")
		{
			cout << "compatible value and DT" << endl;
			return true;
		}
		cout << "Incompatibile DT and Value" << endl;
		return false;
	}

	else if (DT == "char")
	{
		if (valueCP == "char literal")
		{
			cout << "compatible value and DT" << endl;
			return true;
		}
		cout << "Incompatibile DT and Value" << endl;
		return false;
	}

	else if (DT == "string")
	{
		if (valueCP == "string literal")
		{
			cout << "compatible value and DT" << endl;
			return true;
		}
		cout << "Incompatibile DT and Value" << endl;
		return false;
	}

	else if (DT == "bool")
	{
		if (valueCP == "bool literal")
		{
			cout << "compatible value and DT" << endl;
			return true;
		}
		cout << "Incompatibile DT and Value" << endl;
		return false;
	}

	else if (DT == "float")
	{
		if (valueCP == "float literal")
		{
			cout << "compatible value and DT" << endl;
			return true;
		}
		cout << "Incompatibile DT and Value" << endl;
		return false;
	}
}

string Semantics::lookupST(string name, int scope)
{
	if (undeclareFlag == true)
	{	
		for (auto& entry: scopeTable)
		{
			string ID = get<0>(entry);
			int scopeID = get<2>(entry);
			string type = get<1>(entry);
			
			if (name == ID && scope == scopeID)
			{
				return type;
			}
		}
	}
	return "false";
}

void Semantics::createUndeclare(string cp, string vp)
{
	if (undeclareFlag == true)
	{
		int scope = get<2>(undeclareCheck);

		if (scope == 888)
		{
			scope = scopeID;
			get<2>(undeclareCheck) = scope;
		}

		if (cp == "ID")
		{
			get<0>(undeclareCheck) = vp;
			string type = lookupST(vp, scope);
			get<1>(undeclareCheck) = type;
		}

		/*if (cp == "int literal" || cp == "char literal" || cp == "string literal" || cp == "bool literal" || cp == "float literal")
		{
			get<3>(checkDeclare) = cp;
		}*/
	}
}

