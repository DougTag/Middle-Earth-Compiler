#include <bits/stdc++.h> // Mudar depois

using namespace std;

map<string, string> tokenString = {
    {'+', "TK_SUM"},
    {"+=", "TK_ADD_ASSIGN"},
    {"-", "TK_SUB"},
    {"-=", "TK_SUB_ASSING"},
    {"*", "TK_MULT"},
    {"*=", "TK_MULT_ASSIGN"},
    {"**", "TK_POW"},
    {"**=", "TK_POW_ASSIGN"},
    {"/", "TK_DIV"},
    {"/=", "TK_DIV_ASSIGN"},
    {"<<", "TK_SHIFT_LEFT"},
    {">>", "TK_SHIFT_RIGHT"},
    {"=", "TK_ASSIGN"},
    {"==", "TK_EQUAL"},
    {"<", "TK_LESS"},
    {"<=", "TK_LESS_OR_EQUAL"},
    {">", "TK_GREATER"},
    {">=", "TK_GREATER_OR_EQUAL"},
    {"!=", "TK_DIFFERENT"},
    {"~", "TK_BITWISE_NOT"},
    {"|", "TK_BITWISE_OR"},
    {"&", "TK_BITWISE_AND"},
    {"%", "TK_REST"},
    {"%=", "TK_REST_ASSIGN"},
    {"(", "TK_OPEN_PARENTHESIS"},
    {")", "TK_CLOSE_PARENTHESIS"},
    {"{", "TK_OPEN_BRACE"},
    {"}", "TK_CLOSE_BRACE"},
    {";", "TK_SEMICOLON"},  // Verificar se ficou esse nome no automato

    {"elf", "TK_INT"},
    {"draugr", "TK_UNSIGNED"},
    {"hobbit", "TK_SHORT"},
    {"troll", "TK_LONG"},
    {"wyvern", "TK_FLOAT"},
    {"dragon", "TK_DOUBLE"},
    {"istari", "TK_BOOL"},
    {"tengwarr", "TK_CHAR"},
    {"sindarin", "TK_STRING"},
    {"darkness", "TK_VOID"},

    {"The journey begins here", "TK_MAIN"},
    {"Quest", "TK_"},
    {"The quest continues", "TK_"},
    {"You shall not pass", "TK_"},
    {"Otherwise", "TK_"},
    {"Given", "TK_"},
    {"However when", "TK_"},
    {"Go back to the abyss", "TK_RETURN"}


}
    
    

string toToken(string 

int main(){
    ifstream code;
    ofstream token_code;
    code.open("code.mepp");
    token_code.open("token_code.txt");
    set<char> whitespace = {' ', '\f', '\r', '\t', '\v'}; // '\n' nao incluido

    char c;
    int linha = 1, coluna = 0
    while(code >> c){
        ++coluna;
        if(c == '\n')
            ++linha;
        else if(whitespace.count(c) == 1) 
            continue;
        else if(c == '#')
            code.ignore(numeric_limits<streamsize>::max(), '\n');
        else if(c == '+'){
            token_code << token(




    }

    return 0;
}
