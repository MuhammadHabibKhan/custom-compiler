#include "Syntax.h"
#include "Semantic.h"

using namespace std;

int index = 0;

Semantics semantic;

vector <NonTerminals> NT;
vector <string> tokens;
vector <tuple <string, string, int> > TS;

vector <string> valueParts;
vector <int> line;

Syntax::Syntax(vector <tuple <string, string, int> > tokenSet)
{
    for (int i = 0; i < tokenSet.size(); i++)
    {
        string classPart = get<0>(tokenSet[i]);
        tokens.push_back(classPart);
        
        string vp = get<1>(tokenSet[i]);
        valueParts.push_back(vp);

        int ln = get<2>(tokenSet[i]);
        line.push_back(ln);
    }

    TS = tokenSet;
}


void Syntax::createNT()
{
    NonTerminals start;
    start.name = "start";
    start.firstSets = {"class", "interface", "def", "DT", "ID", "list", "set", "void"};
    start.followSets = { "$" };
    start.productionRules = { {"`defs`", "void", "main", "(", "`args`", ")", "{", "`MST`", "}", "`defs`"} };
    NT.push_back(start);

    NonTerminals defs;
    defs.name = "defs";
    defs.firstSets = {"class", "interface", "def", "DT", "ID", "list", "set"};
    defs.followSets = {"void"};
    defs.productionRules = { {"`class`", "`defs`"}, {"`function`", "`defs`"}, {"`declare", "`defs`"}, {"ID", "`declaration`", ";", "`defs`"}, {"`list-decl`", "`defs`"}, {"`set-decl`", "`defs`"} };
    NT.push_back(defs);

    NonTerminals declare;
    declare.name = "declare";
    declare.firstSets = {"DT"};
    declare.productionRules = { {"DT", "ID", "`init", ";"}};
    NT.push_back(declare);

    NonTerminals init;
    init.name = "init";
    init.firstSets = {",", "="};
    init.followSets = {";"};
    init.productionRules = { {",", "ID", "`init`"}, {"=", "`expression`", "`init`"} };
    NT.push_back(init);

    NonTerminals value;
    value.name = "value";
    value.firstSets = {"int literal", "float literal", "char literal", "string literal", "bool literal"};
    value.productionRules = { {"int literal"}, {"float literal"}, {"char literal"}, {"string literal"}, {"bool literal"} };
    NT.push_back(value);

    NonTerminals body;
    body.name = "body";
    body.firstSets = {"{"};
    body.productionRules = { {"{", "`MST`", "}"} };
    NT.push_back(body);

    NonTerminals MST;
    MST.name = "MST";
    MST.firstSets = {"for", "while", "if", "def", "try", "class", "interface", "break", "continue", "return", "ID", "list", "set", "DT"};
    MST.followSets = {"}"};
    MST.productionRules = { {"`SST`", "`MST`"} };
    NT.push_back(MST);

    NonTerminals SST;
    SST.name = "SST";
    SST.firstSets = {"for", "while", "if", "try", "interface", "break", "continue", "return", "ID", "list", "set", "DT" };
    SST.productionRules = {
    {"`for_loop`"},
    {"`while_loop`"},
    {"`conditional_st`"},
    {"`exception_handling`"},
    {"`flow_control`", ";"},
    {"`ret`", ";"},
    {"`declare`"},
    {"`id_decl`"},
    {"`list_decl`"},
    {"`set_decl`"}
    };
    NT.push_back(SST);

    NonTerminals id_decl;
    id_decl.name = "id_decl";
    id_decl.firstSets = { "ID" };
    id_decl.productionRules = { { "ID", "`declaration`", ";" } };
    NT.push_back(id_decl);

    NonTerminals declaration;
    declaration.name = "declaration";
    declaration.firstSets = {".", "(", "=", "[", "ID"};
    declaration.followSets = {";"};
    declaration.productionRules = { {"`decl_left`"}, {"`object_decl`"}, {"=", "`expression`"} };
    NT.push_back(declaration);

    /*NonTerminals decl;
    decl.name = "decl";
    decl.firstSets = {".", "(", "=", "[" };
    decl.followSets = {";"};
    decl.productionRules = { {"`decl_right`"}, {"`decl_left`"} };
    NT.push_back(decl);*/

    NonTerminals object_decl;
    object_decl.name = "object_decl";
    object_decl.firstSets = {"ID"};
    object_decl.productionRules = { {"ID", "`obj`"} };
    NT.push_back(object_decl);

    NonTerminals obj;
    obj.name = "obj";
    obj.firstSets = {"("};
    obj.followSets = {";"};
    obj.productionRules = { {"(", "`parameter`", ")"} };
    NT.push_back(obj);

    NonTerminals decl_left;
    decl_left.name = "decl_left";
    decl_left.firstSets = {"(", ".", "["};
    decl_left.followSets = {"mdm", "pm", "ro", "and", "or", ";", ")", "]", ",", "}"};
    decl_left.productionRules = {
    {"`list_size`", "`list_access`"},
    {"(", "`args`", ")", "`decl_right`"},
    {"decl_right"}
    };
    NT.push_back(decl_left);

    NonTerminals list_access;
    list_access.name = "list_access";
    list_access.firstSets = {".", "="};
    list_access.followSets = {"mdm", "pm", "ro", "and", "or", ";", ")", "]", ",", "}" };
    list_access.productionRules = { {"`decl-right`"}, {"=", "`expression`"} };
    NT.push_back(list_access);

    NonTerminals decl_right;
    decl_right.name = "decl_right";
    decl_right.firstSets = {"."};
    decl_right.followSets = { "mdm", "pm", "ro", "and", "or", ";", ")", "]", ",", "}" };
    decl_right.productionRules = { {".ID", "`decl_left`"} };
    NT.push_back(decl_right);

    NonTerminals list_decl;
    list_decl.name = "list_decl";
    list_decl.firstSets = {"list"};
    list_decl.productionRules = { {"list", "ID", "`list_size`", "`list_end`"} };
    NT.push_back(list_decl);

    NonTerminals list_end;
    list_end.name = "list_end";
    list_end.firstSets = { ";", "="};
    list_end.productionRules = { {";"}, {"=", "`A2`", ";"} };
    NT.push_back(list_end);

    NonTerminals list_size;
    list_size.name = "list_size";
    list_size.firstSets = {"["};
    list_size.productionRules = { {"[", "`expression`", "]", "`list_size1`"} };
    NT.push_back(list_size);

    NonTerminals list_size1;
    list_size1.name = "list_size1";
    list_size1.firstSets = {"["};
    list_size1.followSets = {".", "mdm", "pm", "ro", "and", "or", ";", ")", "]", ",", "}", "="};
    list_size1.productionRules = { {"`list_size`"} };
    NT.push_back(list_size1);

    NonTerminals set_decl;
    set_decl.name = "set_decl";
    set_decl.firstSets = {"set"};
    set_decl.productionRules = { {"set", "`type`", "ID", "[", "`expression`", "]", "`set_init`", ";"} };
    NT.push_back(set_decl);

    NonTerminals set_init;
    set_init.name = "set_init";
    set_init.firstSets = {"="};
    set_init.productionRules = { {"=", "{", "`expression`", "`set_next`", "}"} };
    NT.push_back(set_init);

    NonTerminals set_next;
    set_next.name = "set_next";
    set_next.firstSets = {","};
    set_next.followSets = {"}"};
    set_next.productionRules = { {",", "`expression`", "`set_next`"} };
    NT.push_back(set_next);

    NonTerminals args;
    args.name = "args";
    args.firstSets = { "int literal", "float literal", "char literal", "string literal", "bool literal", "ID", "("};
    args.followSets = {")"};
    args.productionRules = { {"`expression`", "`args_next`"} };
    NT.push_back(args);

    NonTerminals args_next;
    args_next.name = "args_next";
    args_next.firstSets = {","};
    args_next.followSets = {")"};
    args_next.productionRules = { {",", "`args`"} };
    NT.push_back(args_next);

    NonTerminals A2;
    A2.name = "A2";
    A2.firstSets = {"["};
    A2.productionRules = { {"[", "`A3`", "]"} };
    NT.push_back(A2);

    NonTerminals A3;
    A3.name = "A3";
    A3.firstSets = {"[", "ID", "int literal", "float literal", "char literal", "string literal", "bool literal", "(" };
    A3.productionRules = { {"`A2`", "`A4`"}, {"`expression`", "`A4`"} };
    NT.push_back(A3);

    NonTerminals A4;
    A4.name = "A4";
    A4.firstSets = {","};
    A4.followSets = {"]"};
    A4.productionRules = { {",", "`A3`"} };
    NT.push_back(A4);

    NonTerminals conditional_st;
    conditional_st.name = "conditional_st";
    conditional_st.firstSets = {"if"};
    conditional_st.productionRules = { {"if", "(", "`expression`", ")", "`body`", "`Else`"} };
    NT.push_back(conditional_st);

    NonTerminals Else;
    Else.name = "Else";
    Else.firstSets = {"elif", "else"};
    Else.followSets = {"}"};
    Else.productionRules = { {"elif", "(", "`expression`", ")", "`body`", "`Else`"}, {"else", "`body`"} };
    NT.push_back(Else);

    NonTerminals flow_control;
    flow_control.name = "flow_control";
    flow_control.firstSets = {"break", "continue"};
    flow_control.productionRules = { {"break"}, {"continue"} };
    NT.push_back(flow_control);

    NonTerminals function;
    function.name = "function";
    function.firstSets = {  };
    function.followSets = {};
    function.productionRules = {  };
    NT.push_back(function);

    NonTerminals func1;
    func1.name = "func1";
    func1.firstSets = {"DT", "ID", "void"};
    func1.productionRules = { {"DT", "ID", "(", "`parameter`", ")", "`body`"},
                         {"void", "ID", "(", "`parameter`", ")", "`body`"},
                         {"ID", "ID", "(", "`parameter`", ")", "`body`"} };
    NT.push_back(func1);

    NonTerminals parameter;
    parameter.name = "parameter";
    parameter.firstSets = { "DT", "ID"};
    parameter.followSets = { ")" };
    parameter.productionRules = { {"`type`", "ID", "`para1`"} };
    NT.push_back(parameter);

    NonTerminals para1;
    para1.name = "para1";
    para1.firstSets = {","};
    para1.followSets = { ")" };
    para1.productionRules = { {",", "`parameter`"} };
    NT.push_back(para1);

    NonTerminals type;
    type.name = "type";
    type.firstSets = {"DT", "ID"};
    type.productionRules = { {"DT"}, {"ID"} };
    NT.push_back(type);

    NonTerminals ret;
    ret.name = "ret";
    ret.firstSets = {"return"};
    ret.productionRules = { {"ret", "`ret1`"} };
    NT.push_back(ret);

    NonTerminals ret1;
    ret1.name = "ret1";
    ret1.firstSets = {"int literal", "float literal", "char literal", "string literal", "bool literal", "ID", "(" };
    ret1.followSets = { ";" };
    ret1.productionRules = { {"`expression`"} };
    NT.push_back(ret1);

    NonTerminals exception_handling;
    exception_handling.name = "expection_handling";
    exception_handling.firstSets = {"try"};
    exception_handling.productionRules = { {"try", "`body`", "catch", "(", "err", ")", "`body`", "finally", "`body`"} };
    NT.push_back(exception_handling);

    NonTerminals for_loop;
    for_loop.name = "for_loop";
    for_loop.firstSets = {"for"};
    for_loop.productionRules = { {"for", "(", "`f1`", "`f2`", ";", "`f3`", ")", "`body`"} };
    NT.push_back(for_loop);

    NonTerminals f1;
    f1.name = "f1";
    f1.firstSets = { "DT", "ID", ";"};
    //f1.followSets = {"ID", "int literal", "float literal", "char literal", "string literal", "bool literal", ";", "("};
    f1.productionRules = { {"`declare`"}, {"ID", "`declaration`", ";"}, {";"} };
    NT.push_back(f1);

    NonTerminals f2;
    f2.name = "f2";
    f2.firstSets = {"ID", "int literal", "float literal", "char literal", "string literal", "bool literal", "(" };
    f2.followSets = {";"};
    f2.productionRules = { {"`expression`"} };
    NT.push_back(f2);

    NonTerminals f3;
    f3.name = "f3";
    f3.firstSets = {"ID"};
    f3.followSets = {")"};
    f3.productionRules = { {"ID", "=", "`expression`"}};
    NT.push_back(f3);

    NonTerminals while_loop;
    while_loop.name = "while_loop";
    while_loop.firstSets = {"while"};
    while_loop.productionRules = { {"while", "(", "`expression`", ")", "`body`"} };
    NT.push_back(while_loop);

    NonTerminals expression;
    expression.name = "expression";
    expression.firstSets = {"ID", "int literal", "float literal", "char literal", "string literal", "bool literal", "("};
    expression.productionRules = { {"`AND`", "`OR`"} };
    NT.push_back(expression);

    NonTerminals OR;
    OR.name = "OR";
    OR.firstSets = {"or"};
    OR.followSets = {";", ")", "]", ","};
    OR.productionRules = { {"or", "`AND`", "`OR`"} };
    NT.push_back(OR);

    NonTerminals AND;
    AND.name = "AND";
    AND.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal", "(" };
    AND.productionRules = { {"`RO`", "`AND1`"} };
    NT.push_back(AND);

    NonTerminals AND1;
    AND1.name = "AND1";
    AND1.firstSets = {"and"};
    AND1.followSets = { ";", ")", "]", ",", "or"};
    AND1.productionRules = { {"and", "`RO`", "`AND1`"} };
    NT.push_back(AND1);

    NonTerminals RO;
    RO.name = "RO";
    RO.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal", "(" };
    RO.productionRules = { {"`PM`", "`RO1`"} };
    NT.push_back(RO);

    NonTerminals RO1;
    RO1.name = "RO1";
    RO1.firstSets = {"ro"};
    RO1.followSets = { ";", ")", "]", ",", "or", "and"};
    RO1.productionRules = { {"ro", "`PM`", "`RO1`"} };
    NT.push_back(RO1);

    NonTerminals PM;
    PM.name = "PM";
    PM.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal", "(" };
    PM.productionRules = { {"`MDM`", "`PM1`"} };
    NT.push_back(PM);

    NonTerminals PM1;
    PM1.name = "PM1";
    PM1.firstSets = {"pm"};
    PM1.followSets = { ";", ")", "]", ",", "or", "and", "ro"};
    PM1.productionRules = { {"pm", "`MDM`", "`PM1`"} };
    NT.push_back(PM1);

    NonTerminals MDM;
    MDM.name = "MDM";
    MDM.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal", "(" };
    MDM.productionRules = { {"`END`", "`MDM1`"} };
    NT.push_back(MDM);

    NonTerminals MDM1;
    MDM1.name = "MDM1";
    MDM1.firstSets = {"mdm"};
    MDM1.followSets = { ";", ")", "]", ",", "or", "and", "ro", "pm"};
    MDM1.productionRules = { {"mdm", "`END`", "`MDM1`"} };
    NT.push_back(MDM1);

    NonTerminals END;
    END.name = "END";
    END.firstSets = { "(", "ID", "int literal", "float literal", "char literal", "string literal", "bool literal", "(" };
    END.productionRules = { {"`value`"}, {"ID", "`exp_decl`"}, {"(", "`expression`", ")"} };
    NT.push_back(END);

    NonTerminals exp_decl;
    exp_decl.name = "exp_decl";
    exp_decl.firstSets = {"(", "[", "."};
    exp_decl.followSets = { ";", ")", "]", ",", "or", "and", "ro", "pm", "mdm" };
    exp_decl.productionRules = { {"(", "`args`", ")", "`decl_right`"}, {"`list_size`", "`decl_right`"}, {"`decl_right`"}};
    NT.push_back(exp_decl);


    NonTerminals structure;
    structure.name = "structure";
    structure.firstSets = {"class", "interface"};
    structure.productionRules = { {"class"}, {"interface"} };
    NT.push_back(structure);

    // For the "class" CFG
    NonTerminals class_nt;
    class_nt.name = "class";
    class_nt.firstSets = { "class", "interface" };
    class_nt.productionRules = { {"`structure`", "ID", "`inherit`", "`access_modifier`", "`class_outer_body`"} };
    NT.push_back(class_nt);

    // For the "inherit" CFG
    NonTerminals inherit;
    inherit.name = "inherit";
    inherit.firstSets = {"from"};
    inherit.followSets = {"`access_modifier`", "{"};
    inherit.productionRules = { {"from", "ID", "`inherit`"} };
    NT.push_back(inherit);

    // For the "access_modifier" CFG
    NonTerminals access_modifier;
    access_modifier.name = "access_modifier";
    access_modifier.firstSets = {"access-modifier"};
    access_modifier.followSets = {"{", "def", "DT", "ID", "constructor", "list", "set" };
    access_modifier.productionRules = { {"access-modifier", ":"} };
    NT.push_back(access_modifier);

    // For the "class_outer_body" CFG
    NonTerminals class_outer_body;
    class_outer_body.name = "class_outer_body";
    class_outer_body.firstSets = {"{"};
    class_outer_body.productionRules = { {"{", "`class_outer_MST`", "}"} };
    NT.push_back(class_outer_body);

    // For the "class_outer_MST" CFG
    NonTerminals class_outer_MST;
    class_outer_MST.name = "class_outer_MST";
    class_outer_MST.firstSets = {"def", "DT", "ID", "constructor", "list", "set", "access-modifier"};
    class_outer_MST.followSets = {"}"};
    class_outer_MST.productionRules = { {"`access_modifier`", "`class_outer_SST`", "`class_outer_MST`"} };
    NT.push_back(class_outer_MST);

    // For the "class_outer_SST" CFG
    NonTerminals class_outer_SST;
    class_outer_SST.name = "class_outer_SST";
    class_outer_SST.firstSets = { "def", "DT", "ID", "constructor", "list", "set" };
    class_outer_SST.productionRules = {
        {"`class_function`"},
        {"`class_declare`"},
        {"ID", "`object_decl`", ";"},
        {"`class_constructor`"},
        {"`class_list_decl`"},
        {"`class_set_decl`"}
    };
    NT.push_back(class_outer_SST);

    // For the "class_declare" CFG
    NonTerminals class_declare;
    class_declare.name = "class_declare";
    class_declare.firstSets = {"DT"};
    class_declare.productionRules = { {"DT", "ID", "`class_init`", ";"} };
    NT.push_back(class_declare);

    // For the "class_init" CFG
    NonTerminals class_init;
    class_init.name = "class_init";
    class_init.firstSets = {",", "="};
    class_init.followSets = {";"};
    class_init.productionRules = { {",", "ID", "`class_init`"}, {"=", "`class_expression`", "`class_init`"} };
    NT.push_back(class_init);


    // For the "fn" CFG
    NonTerminals fn;
    fn.name = "fn";
    fn.firstSets = {"static"};
    fn.followSets = {"DT", "void", "ID"};
    fn.productionRules = { {"static"} };
    NT.push_back(fn);

    // For the "class_function" CFG
    NonTerminals class_function;
    class_function.name = "class_function";
    class_function.firstSets = {"def"};
    class_function.productionRules = { {"def", "`fn`", "`class_func1`"} };
    NT.push_back(class_function);

    // For the "class_func1" CFG
    NonTerminals class_func1;
    class_func1.name = "class_func1";
    class_func1.firstSets = {"DT", "void", "ID"};
    class_func1.productionRules = {
        {"DT", "`a`", "(", "`parameter`", ")", "`class_body`"},
        {"void", "`a`", "(", "`parameter`", ")", "`class_body`"},
        {"ID", "`a`", "(", "`parameter`", ")", "`class_body`"}
    };
    NT.push_back(class_func1);

    // For the "a" CFG
    NonTerminals a;
    a.name = "a";
    a.firstSets = {"ID", "operator"};
    a.productionRules = { {"ID"}, {"operator", "`op`"} };
    NT.push_back(a);

    // For the "op" CFG
    NonTerminals op;
    op.name = "op";
    op.firstSets = {"and", "or", "pm", "ro", "mdm"};
    op.productionRules = { {"PM"}, {"MDM"}, {"RO"}, {"and"}, {"or"} };
    NT.push_back(op);

    // For the "class_constructor" CFG
    NonTerminals class_constructor;
    class_constructor.name = "class_constructor";
    class_constructor.firstSets = {"constructor"};
    class_constructor.productionRules = { {"constructor", "ID", "(", "`parameter`", ")", "`class_body`"} };
    NT.push_back(class_constructor);

    // For the "class_body" CFG
    NonTerminals class_body;
    class_body.name = "class_body";
    class_body.firstSets = {"{"};
    class_body.productionRules = { {"{", "`class_MST`", "}"} };
    NT.push_back(class_body);

    // For the "class_MST" CFG
    NonTerminals class_MST;
    class_MST.name = "class_MST";
    class_MST.firstSets = {"return", "for", "if", "while", "try", "break", "continue", "DT", "list", "set", "this", "super", "ID"};
    class_MST.followSets = {"}"};
    class_MST.productionRules = { {"`class_SST`", "`class_MST`"} };
    NT.push_back(class_MST);

    // For the "class_SST" CFG
    NonTerminals class_SST;
    class_SST.name = "class_SST";
    class_SST.firstSets = { "return", "for", "if", "while", "try", "break", "continue", "DT", "list", "set", "this", "super", "ID" };
    class_SST.productionRules = {
        {"`class_return`", ";"},
        {"`class_for_loop`"},
        {"`class_conditional_st`"},
        {"`class_while_loop`"},
        {"`class_exception_handling`"},
        {"`flow_control`", ";"},
        {"`class_declare`"},
        {"`class_list_decl`"},
        {"`class_set_decl`"},
        {"`super_this`", "`this_decl`", ";"},
        {"ID", "`class_declaration`", ";"}
    };
    NT.push_back(class_SST);

    // For the "super_this" CFG
    NonTerminals super_this;
    super_this.name = "super_this";
    super_this.firstSets = {"super", "this"};
    super_this.productionRules = { {"this", ".ID"}, {"super", ".ID"} };
    NT.push_back(super_this);

    // For the "this_decl" CFG
    NonTerminals this_decl;
    this_decl.name = "this_decl";
    this_decl.firstSets = {".", "(", "[", "="};
    this_decl.followSets = {"}"};
    this_decl.productionRules = { {"`class_decl_left`"}, {"=", "`expression`"} };
    NT.push_back(this_decl);

    // For the "class_declaration" CFG
    NonTerminals class_declaration;
    class_declaration.name = "class_declaration";
    class_declaration.firstSets = { ".", "(", "=", "[", "ID" };
    class_declaration.followSets = {";"};
    class_declaration.productionRules = { {"`class_decl_left`"}, {"`object_decl`"}, {"=", "`expression`"} };
    NT.push_back(class_declaration);

    // For the "class_decl_left" CFG
    NonTerminals class_decl_left;
    class_decl_left.name = "class_decl_left";
    class_decl_left.firstSets = { ".", "(", "[" };
    class_decl_left.followSets = {";", "mdm", "pm", "ro", "and", "or", ")", "]", ",", "}"};
    class_decl_left.productionRules = {
        {"(`class_args`)", "`class_decl_right`"},
        {"`class_list_size`", "`class_list_access`"},
        {"`class_decl_right`"}
    };
    NT.push_back(class_decl_left);

    // For the "class_list_access" CFG
    NonTerminals class_list_access;
    class_list_access.name = "class_list_access";
    class_list_access.firstSets = {",", "="};
    class_list_access.followSets = { ";", "mdm", "pm", "ro", "and", "or", ")", "]", ",", "}" };
    class_list_access.productionRules = { {"`class_decl_right`"}, {"=", "`class_expression`"} };
    NT.push_back(class_list_access);

    // For the "class_decl_right" CFG
    NonTerminals class_decl_right;
    class_decl_right.name = "class_decl_right";
    class_decl_right.firstSets = {"."};
    class_decl_right.followSets = { ";", "mdm", "pm", "ro", "and", "or", ")", "]", ",", "}" };
    class_decl_right.productionRules = { {".ID", "`class_decl_left`"} };
    NT.push_back(class_decl_right);

    // For the "class_list_decl" CFG
    NonTerminals class_list_decl;
    class_list_decl.name = "class_list_decl";
    class_list_decl.firstSets = {"list"};
    class_list_decl.productionRules = { {"list", "ID", "`class_list_size`", "`class_list_end`"} };
    NT.push_back(class_list_decl);

    // For the "class_list_end" CFG
    NonTerminals class_list_end;
    class_list_end.name = "class_list_end";
    class_list_end.firstSets = {";", "="};
    class_list_end.productionRules = { {";"}, {"=", "`class_A2`", ";"} };
    NT.push_back(class_list_end);

    // For the "class_list_size" CFG
    NonTerminals class_list_size;
    class_list_size.name = "class_list_size";
    class_list_size.firstSets = {"["};
    class_list_size.productionRules = { {"[", "`class_expression`", "]", "`class_list_size1`"} };
    NT.push_back(class_list_size);

    // For the "class_list_size1" CFG
    NonTerminals class_list_size1;
    class_list_size1.name = "class_list_size1";
    class_list_size1.firstSets = {"["};
    class_list_size1.followSets = { ";", ".", "mdm", "pm", "ro", " and ", " or ", ")", "]", ",", "}", " = "};
    class_list_size1.productionRules = { {"`class_list_size`"} };
    NT.push_back(class_list_size1);

    // For the "class_set_decl" CFG
    NonTerminals class_set_decl;
    class_set_decl.name = "class_set_decl";
    class_set_decl.firstSets = {"set"};
    class_set_decl.productionRules = { {"set", "`type`", "ID", "[", "`class_expression`", "]", "`class_set_init`", ";"} };
    NT.push_back(class_set_decl);

    // For the "class_set_init" CFG
    NonTerminals class_set_init;
    class_set_init.name = "class_set_init";
    class_set_init.firstSets = {"="};
    class_set_init.productionRules = { {"=", "{", "`class_expression`", "`class_set_next`", "}"} };
    NT.push_back(class_set_init);

    // For the "class_set_next" CFG
    NonTerminals class_set_next;
    class_set_next.name = "class_set_next";
    class_set_next.firstSets = {","};
    class_set_next.followSets = {"}"};
    class_set_next.productionRules = { {",", "`class_expression`", "`class_set_next`"} };
    NT.push_back(class_set_next);

    // For the "class_args" CFG
    NonTerminals class_args;
    class_args.name = "class_args";
    class_args.firstSets = {"ID", "int literal", "float literal", "char literal", "string literal", "bool literal" };
    class_args.followSets = {")"};
    class_args.productionRules = { {"`class_expression`", "`class_args_next`"} };
    NT.push_back(class_args);

    // For the "class_args_next" CFG
    NonTerminals class_args_next;
    class_args_next.name = "class_args_next";
    class_args_next.firstSets = {","};
    class_args_next.followSets = {")"};
    class_args_next.productionRules = { {",", "`class_args`"} };
    NT.push_back(class_args_next);

    // For the "class_A2" CFG
    NonTerminals class_A2;
    class_A2.name = "class_A2";
    class_A2.firstSets = {"["};
    class_A2.productionRules = { {"[", "`class_A3`", "]"} };
    NT.push_back(class_A2);

    // For the "class_A3" CFG
    NonTerminals class_A3;
    class_A3.name = "class_A3";
    class_A3.firstSets = {"[", "ID", "(", "int literal", "float literal", "char literal", "string literal", "bool literal" };
    class_A3.productionRules = { {"`class_A2`", "`class_A4`"}, {"`class_expression`", "`class_A4`"} };
    NT.push_back(class_A3);

    // For the "class_A4" CFG
    NonTerminals class_A4;
    class_A4.name = "class_A4";
    class_A4.firstSets = {","};
    class_A4.followSets = {"]"};
    class_A4.productionRules = { {",", "`class_A3`"} };
    NT.push_back(class_A4);

    // For the "class_conditional_st" CFG
    NonTerminals class_conditional_st;
    class_conditional_st.name = "class_conditional_st";
    class_conditional_st.firstSets = {"if"};
    class_conditional_st.productionRules = { {"if", "(", "`class_expression`", ")", "`class_body`", "`class_Else`"} };
    NT.push_back(class_conditional_st);

    // For the "class_Else" CFG
    NonTerminals class_Else;
    class_Else.name = "class_Else";
    class_Else.firstSets = {"else", "elif"};
    class_Else.followSets = {"}"};
    class_Else.productionRules = { {"elif", "(", "`class_expression`", ")", "`class_body`", "`class_Else`"}, {"else", "`class_body`"} };
    NT.push_back(class_Else);

    // For the "class_return" CFG
    NonTerminals class_return;
    class_return.name = "class_return";
    class_return.firstSets = {"return"};
    class_return.productionRules = { {"return", "`class_ret1`"} };
    NT.push_back(class_return);

    // For the "class_ret1" CFG
    NonTerminals class_ret1;
    class_ret1.name = "class_ret1";
    class_ret1.firstSets = {"ID", "int literal", "float literal", "char literal", "string literal", "bool literal" , "("};
    class_ret1.followSets = {";"};
    class_ret1.productionRules = { {"`class_expression`"} };
    NT.push_back(class_ret1);

    // For the "class_exception_handling" CFG
    NonTerminals class_exception_handling;
    class_exception_handling.name = "class_exception_handling";
    class_exception_handling.firstSets = {"try"};
    class_exception_handling.productionRules = { {"try", "`class_body`", "catch", "(", "err", ")", "`class_body`", "finally", "`class_body`"} };
    NT.push_back(class_exception_handling);

    // For the "class_for_loop" CFG
    NonTerminals class_for_loop;
    class_for_loop.name = "class_for_loop";
    class_for_loop.firstSets = {"for"};
    class_for_loop.productionRules = { {"for", "(", "`class_f1`", "`class_f2`", ";", "`class_f3`", ")", "`class_body`"} };
    NT.push_back(class_for_loop);

    // For the "class_f1" CFG
    NonTerminals class_f1;
    class_f1.name = "class_f1";
    class_f1.firstSets = {"DT", "ID", ";"};
    //class_f1.followSets = {"ID", "int literal", "float literal", "char literal", "string literal", "bool literal" , ";"};
    class_f1.productionRules = { {"`class_declare`"}, {"ID", "`class_declaration`", ";"}, {";"} };
    NT.push_back(class_f1);

    // For the "class_f2" CFG
    NonTerminals class_f2;
    class_f2.name = "class_f2";
    class_f2.firstSets = {"ID", "int literal", "float literal", "char literal", "string literal", "bool literal" , "("};
    class_f2.followSets = {";"};
    class_f2.productionRules = { {"`class_expression`"} };
    NT.push_back(class_f2);

    // For the "class_f3" CFG
    NonTerminals class_f3;
    class_f3.name = "class_f3";
    class_f3.firstSets = {"ID"};
    class_f3.followSets = {")"};
    class_f3.productionRules = { {"ID", "=", "`class_expression`"} };
    NT.push_back(class_f3);

    // For the "class_while_loop" CFG
    NonTerminals class_while_loop;
    class_while_loop.name = "class_while_loop";
    class_while_loop.firstSets = {"while"};
    class_while_loop.productionRules = { {"while", "(", "`class_expression`", ")", "`class_body`"} };
    NT.push_back(class_while_loop);

    // For the "class_expression" CFG
    NonTerminals class_expression;
    class_expression.name = "class_expression";
    class_expression.firstSets = { "int literal", "float literal", "char literal", "string literal", "bool literal" , "ID", "this", "super", "("};
    class_expression.productionRules = { {"`class_AND`", "`class_OR`"} };
    NT.push_back(class_expression);

    // For the "class_OR" CFG
    NonTerminals class_OR;
    class_OR.name = "class_OR";
    class_OR.firstSets = {"or"};
    class_OR.followSets = {";", ")", "}", "]", ","};
    class_OR.productionRules = { {"or", "`class_AND`", "`class_OR`"} };
    NT.push_back(class_OR);

    // For the "class_AND" CFG
    NonTerminals class_AND;
    class_AND.name = "class_AND";
    class_AND.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal" , "(" };
    class_AND.productionRules = { {"`class_RO`", "`class_AND1`"} };
    NT.push_back(class_AND);

    // For the "class_AND1" CFG
    NonTerminals class_AND1;
    class_AND1.name = "class_AND1";
    class_AND1.firstSets = {"and"};
    class_AND1.followSets = { ";", ")", "}", "]", ",", "or"};
    class_AND1.productionRules = { {"and", "`class_RO`", "`class_AND1`"} };
    NT.push_back(class_AND1);

    // For the "class_RO" CFG
    NonTerminals class_RO;
    class_RO.name = "class_RO";
    class_RO.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal" , "(" };
    class_RO.productionRules = { {"`class_PM`", "`class_RO1`"} };
    NT.push_back(class_RO);

    // For the "class_RO1" CFG
    NonTerminals class_RO1;
    class_RO1.name = "class_RO1";
    class_RO1.firstSets = {"ro"};
    class_RO1.followSets = { ";", ")", "}", "]", ",", "or", "and"};
    class_RO1.productionRules = { {"RO", "`class_PM`", "`class_RO1`"} };
    NT.push_back(class_RO1);

    // For the "class_PM" CFG
    NonTerminals class_PM;
    class_PM.name = "class_PM";
    class_PM.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal" , "(" };
    class_PM.productionRules = { {"`class_MDM`", "`class_PM1`"} };
    NT.push_back(class_PM);

    // For the "class_PM1" CFG
    NonTerminals class_PM1;
    class_PM1.name = "class_PM1";
    class_PM1.firstSets = {"pm"};
    class_PM1.followSets = { ";", ")", "}", "]", ",", "or", "and", "ro"};
    class_PM1.productionRules = { {"pm", "`class_MDM`", "`class_PM1`"} };
    NT.push_back(class_PM1);

    // For the "class_MDM" CFG
    NonTerminals class_MDM;
    class_MDM.name = "class_MDM";
    class_MDM.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal" , "(" };
    class_MDM.productionRules = { {"`class_END`", "`class_MDM1`"} };
    NT.push_back(class_MDM);

    // For the "class_MDM1" CFG
    NonTerminals class_MDM1;
    class_MDM1.name = "class_MDM1";
    class_MDM1.firstSets = {"mdm"};
    class_MDM1.followSets = { ";", ")", "}", "]", ",", "or", "and", "ro", "pm"};
    class_MDM1.productionRules = { {"mdm", "`class_END`", "`class_MDM1`"} };
    NT.push_back(class_MDM1);

    // For the "class_END" CFG
    NonTerminals class_END;
    class_END.name = "class_END";
    class_END.firstSets = { "ID", "int literal", "float literal", "char literal", "string literal", "bool literal" , "(", "this", "super"};
    class_END.productionRules = { {"`value`"}, {"ID", "`class_exp_decl`"}, {"(", "`class_expression`", ")"}, {"`super_this`", "`class_exp_decl`"} };
    NT.push_back(class_END);

    // For the "class_exp_decl" CFG
    NonTerminals class_exp_decl;
    class_exp_decl.name = "class_exp_decl";
    class_exp_decl.firstSets = {"(", "[", "."};
    class_exp_decl.followSets = { ";", ")", "}", "]", ",", "or", "and", "ro", "pm", "mdm"};
    class_exp_decl.productionRules = { {"(`class_args`)", "`class_decl_right`"}, {"`class_list_size`", "`class_decl_right`"}, {"`class_decl_right`"} };
    NT.push_back(class_exp_decl);

}


NonTerminals NonTerminals::searchNT(string s)
{
    for (auto& nt : NT)
    {
        if (nt.name == s)
        {
            return nt;
        }
    }
}


void Syntax::analyzer()
{
    bool matched = NT[0].match(tokens);

    if (matched)
    {
        cout << get<0>(TS[index]) << endl;

        if (get<0> (TS[index]) == "$") 
        {
            cout << "Syntax validated" << endl;
        }
        else
        {
            cout << "Syntax Error at line " << get<2>(TS[index]);
        }
    }
    else
    {
        cout << get<0>(TS[index]) << endl;
        cout << "Syntax Error at line " << get<2>(TS[index]);
    }
}


string NonTerminals::extractNT(string nt)
{
    string tempNT = "";

    for (int s = 0; s < nt.length(); s++)
    {
        if (nt[s] == '`') {
            continue;
        }
        else
        {
            tempNT += nt[s];
        }
    }
    return tempNT;
}


bool NonTerminals::checkProduction(vector <string> classPart)
{
    string rule;
    
    for (int i = 0; i < this->productionRules.size(); i++)
    {
        for (int j = 0; j < this->productionRules[i].size(); j++)
        {
            // condition for NT in production rule

            if (this->productionRules[i][j][0] == '`')
            {
                rule = extractNT(this->productionRules[i][j]);
                //cout << rule << " extracted" << endl;
                NonTerminals NT = searchNT(rule);

                semantic.flagHandler(rule);
                
                if (NT.match(classPart))
                {
                    if (j == this->productionRules[i].size() - 1)
                    {
                        semantic.handleDeclare(rule);
                        semantic.deFlagger(rule);

                        cout << this->name << " rule validated" << endl;
                        return true;
                    }
                    continue;
                }
                else 
                {
                    break;
                }
            }

            else if (this->productionRules[i][j] == classPart[index])
            {
                // scope create and destroy
                semantic.createScope(classPart[index]);
                semantic.destroyScope(classPart[index]);

                // create ID declaration tuple
                semantic.createDeclare(classPart[index], valueParts[index]);

                cout << classPart[index] << " matched" << endl;
                index++;

                // if last word reached in the rule and verified

                if (j == this->productionRules[i].size() - 1)
                {
                    semantic.handleDeclare(rule);
                    semantic.deFlagger(rule);

                    cout << this->name << " rule validated" << endl;
                    return true;
                }
                continue;
            }
            else
            {
                break;
            }
        }
    }
    return false;
}

bool NonTerminals::match(vector <string> classPart)
{
    // check in first set

    bool matched = false;
    bool follow = false;

    for (int i = 0; i < this->firstSets.size(); i++)
    {
        if (classPart[index] == firstSets[i])
        {
            matched = checkProduction(classPart);
            cout << "matched: " << matched << endl;
            break;
        }
    }

    if (matched == false)
    {
        // check in follow set

        for (int i = 0; i < this->followSets.size(); i++)
        {
            if (classPart[index] == followSets[i])
            {
                follow = true;
            }
        }

        if (follow == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}