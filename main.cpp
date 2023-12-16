#include "Lexical.h"
#include "Syntax.h"

using namespace std;

// ADD CHECKS FOR OTHER FUNCTIONS IN TOKENIZATION AS WELL FOR PREV DOUBLEPREV AND NEXT CHAR ELSE VS THROW ERROR

int main() {

    Lexical analyzer;

    analyzer.createToken();
    analyzer.assignClassPart();

    // adding end marker 
    analyzer.token.push_back(make_tuple("$", "$", 100));

    analyzer.writeTokens();

    Syntax syntax(analyzer.token);
    syntax.createNT();
    syntax.analyzer();

    return 0;
}