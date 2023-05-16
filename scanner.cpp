#include <bits/stdc++.h> // Mudar depois
#define MAX_STRING_SIZE 255

using namespace std;

set<string> operations = {
    "+=", "%=", "-=", "*=", "/=", "**", "<=", ">=", "==", "!=", "**=",
    "+", "-", "*", "/", "=", "<", ">", "~", "|", "&", "%", "(", ")", 
    "{", "}", ";"
}

map<string, string> tokenString = {
    {"+=", "TK_ADD_ASSIGN"},
    {"%=", "TK_REST_ASSIGN"},
    {"-=", "TK_SUB_ASSING"},
    {"*=", "TK_MULT_ASSIGN"},
    {"/=", "TK_DIV_ASSIGN"},
    {"**", "TK_POW"},
    {"<=", "TK_LESS_OR_EQUAL"},
    {">=", "TK_GREATER_OR_EQUAL"},
    {"==", "TK_EQUAL"},
    {"!=", "TK_DIFFERENT"},
    {"**=", "TK_POW_ASSIGN"},
    {"+", "TK_SUM"},
    {"-", "TK_SUB"},
    {"*", "TK_MULT"},
    {"/", "TK_DIV"},
    {"=", "TK_ASSIGN"},
    {"<", "TK_LESS"},
    {">", "TK_GREATER"},
    {"~", "TK_BITWISE_NOT"},
    {"|", "TK_BITWISE_OR"},
    {"&", "TK_BITWISE_AND"},
    {"%", "TK_REST"},
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
    {"Quest", "TK_WHILE"},
    {"The quest continues", "TK_CONTINUE"},
    {"You shall not pass", "TK_BREAK"},
    {"Given", "TK_IF"},
    {"However when", "TK_ELIF"},
    {"Otherwise", "TK_ELSE"},
    {"Go back to the abyss", "TK_RETURN"}

}

string token(string s, unsigned linha, unsigned coluna);
string token(string s, unsigned linha, unsigned coluna, string valor);

int main(){
    ifstream code;
    ofstream token_code;
    code.open("code.mepp");
    token_code.open("token_code.txt", ofstream::out | ofstream::trunc);
    set<char> whitespace = {' ', '\f', '\r', '\t', '\v'}; // '\n' nao incluido

    string s;
    int linha = 1, coluna = 0;
    while(code.read(s, 1)){
        while(operations.count(s+code.peek()) == 1){
            char cAux;
            code >> cAux;
            s.append(cAux);
        }

        if(c == "\n"){
            ++linha;
            coluna = 0;
        }
        else if(whitespace.count(s) == 1) 
            continue;
        else if(s == "#"){
            code.ignore(numeric_limits<streamsize>::max(), "\n");
            ++linha;
            coluna = 0;
        }
        else if(operations.count(s) == 1)
            token_code << token(s, linha, coluna);
        else if(s == "\'"){
            string sAux;
            code.read(sAux, 2);
            s.append(sAux);
            if(s[2] != '\''); // throw
            token_code << token(s, linha, coluna);
        }
        else if(s == "\""){
            code.getline(s.begin()+1, string::max_size, '\"');
            if(code.eof()); // throw string without ending "
	    if(s.size() > MAX_STRING_SIZE); // throw max size string
            s.append("\"");
            token_code << token(s, linha, coluna);
        }
	else if(s >= "1" && s <= "9"){
            char aux;
            while(code >> aux, aux >= '0' && aux <= '9') 
                s.append(aux);
            token_code << token(s, linha, coluna);
	}
        else if(s == "0"){
            char aux;
            if(code.peek() == 'x' || code.peek() == 'X'){   // Se for hexadecimal
                code >> aux;
                s.append(toupper(aux)); // faz x minusculo virar maiusculo 
                while(code >> aux, aux >= '0' && aux <= '9' || aux >= 'a' && aux <= 'f' || aux >= 'A' && aux <= 'F')
                    s.append(toupper(aux));
            }
            if(code.peek() == 'b' || code.peek() == 'B'){   // Se for binario
                code >> aux;
                s.append(toupper(aux)); // faz x minusculo virar maiusculo 
                while(code >> aux, aux == '0' || aux == '1')
                    s.append(aux);
            }
            else{                                           // Se for octal
                while(code >> aux, aux >= '0' && aux <= '7')
                    s.append(aux);
            }
            token_code << token(s, linha, coluna)
        }

        coluna += s.size();
    }

    return 0;
}
