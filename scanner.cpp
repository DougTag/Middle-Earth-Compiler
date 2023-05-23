#include <bits/stdc++.h> // Mudar depois
#include "trie.h"
#define MAX_STRING_SIZE 255

using namespace std;

class Scanner {
private:
    ifstream input;
    int errorCount = 0, line = 1, column = 1;
    const unordered_set<string> operators = { // Set com todos os operators
        "+", "-", "*", "/", "%", "**", "~", "|", "&", "^",
        "+=", "-=", "*=", "/=", "%=", "**=", "~=", "|=", "&=", "^=", "=",
        "==", "!=", "<", ">", "<=", ">=",
        "(", ")", "{", "}", ",", ";"
    };
    const unordered_set<char> operatorsCharacters = { // Set com os caracteres permitidos para os operators
        '=', '+', '-', '*', '/', '%', '~', '|', '&', '^',
        '!', '>', '<', '(', ')', '{', '}', ',', ';'
    };
    const unordered_set<string> keywords = { // Set com as palavras/frases reservadas
        "or", "and", "not", "xor", "good", "evil",
        "draugr", "hobbit", "troll", "wyvern", "dragon", "istari", "tengwarr", "sindarin", "darkness",
        "The journey begins here", "Quest", "The quest continues", "You shall not pass!", "Given",
        "However when", "Otherwise", "go back to the abyss!"
    };
    const unordered_map<string, string> tokenString = { // Map das cadeias para os respectivos tokens
        {"+", "TK_ADD"},
        {"-", "TK_SUB"},
        {"*", "TK_MULT"},
        {"/", "TK_DIV"},
        {"%", "TK_REST"},
        {"**", "TK_POW"},
        {"~", "TK_BITWISE_NOT"},
        {"|", "TK_BITWISE_OR"},
        {"&", "TK_BITWISE_AND"},
        {"^", "TK_BITWISE_XOR"},

        {"=", "TK_ASSIGN"},
        {"+=", "TK_ADD_ASSIGN"},
        {"-=", "TK_SUB_ASSING"},
        {"*=", "TK_MULT_ASSIGN"},
        {"/=", "TK_DIV_ASSIGN"},
        {"%=", "TK_REST_ASSIGN"},
        {"**=", "TK_POW_ASSIGN"},
        {"~=", "TK_BITWISE_NOT_ASSIGN"},
        {"|=", "TK_BITWISE_OR_ASSIGN"},
        {"&=", "TK_BITWISE_AND_ASSIGN"},
        {"^=", "TK_BITWISE_XOR_ASSIGN"},

        {"==", "TK_EQUAL"},
        {"!=", "TK_DIFFERENT"},
        {"<", "TK_LESS"},
        {"<=", "TK_LESS_EQUAL"},
        {">", "TK_GREATER"},
        {">=", "TK_GREATER_EQUAL"},

        {"(", "TK_OPEN_PARENTHESIS"},
        {")", "TK_CLOSE_PARENTHESIS"},
        {"{", "TK_OPEN_BRACE"},
        {"}", "TK_CLOSE_BRACE"},
        {";", "TK_SEMICOLON"},
        {",", "TK_COMMA"},

        {"or", "TK_OR"},
        {"and", "TK_AND"},
        {"not", "TK_NOT"},
        {"xor", "TK_XOR"},

        {"good", "TK_TRUE"},
        {"evil", "TK_FALSE"},

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
        {"You shall not pass!", "TK_BREAK"},
        {"Given", "TK_IF"},
        {"However when", "TK_ELSE_IF"},
        {"Otherwise", "TK_ELSE"},
        {"go back to the abyss!", "TK_RETURN"}
    };
    const unordered_set<char> whitespace = { ' ', '\f', '\r', '\t', '\v' }; // Set com todos os whitespaces. \n tratado separadamente.
    Trie tree = {
        "or", "and", "not", "xor", "good", "evil",
        "draugr", "hobbit", "troll", "wyvern", "dragon", "istari", "tengwarr", "sindarin", "darkness",
        "The journey begins here", "Quest", "The quest continues", "You shall not pass!", "Given",
        "However when", "Otherwise", "go back to the abyss!"
    };

    string formToken(const string& tk, const string& value);
    void skipWhitespace();

public:
    Scanner(const string &fileName);
    bool eof();
    pair<string,string> getToken();
};

/*********************************************/

void Scanner::skipWhitespace() {
    while (whitespace.count(input.peek()) == 1) {
        input.ignore(1);
        ++column;
    }
}

string Scanner::formToken(const string &tk, const string &value = "") {
    return tk + "," + to_string(line) + "," + to_string(column) + "," + value + "\n";
}

Scanner::Scanner(const string &fileName) {
    input.exceptions(ifstream::failbit | ifstream::badbit); // Se houver qualquer erro na abertura
                                                            // ou leitura, da throw ifstream::failure e deixa para a main tratar
    input.open(fileName);
    errorCount = 0;
    line = column = 1;
}

bool Scanner::eof() { 
    skipWhitespace();
    return input.eof();
}

pair<string,string> Scanner::getToken() {
    skipWhitespace();
    if(this->eof()) {
        throw out_of_range("Scanner exception - getToken was called when EOF was reached already.");
    }


    string token;       // Onde o token eh escrito
    stringstream eout;  // Error output, assim como cout eh character output :)
                        // Os erros sao escritos aqui, para serem entregues na saida do metodo.

    char c = input.peek();

    if (c == '\n' or c == '#') {
        input.ignore(numeric_limits<streamsize>::max(), '\n');
        ++line;
        column = 1;
    }
    else if (operatorsCharacters.count(c) == 1) { // Se for um dos caracteres das transicoes de S2 e S3
        input >> c;
        string s = {1, c};
        while (operators.count(s + char(input.peek())) == 1) {  // Esse while pega o proximo caractere se, ao adicionar ele,
            input >> c;                                         // eh formado um outro operator
            s += c;
        }

        if (operators.count(s) == 0) {
            ++errorCount;
            eout << "[SCANNER] ERROR at (lin " << line << ", col " << column << "): ";
            eout << "Unknown operator. This happens when there's a sequence of characters that";
            eout << "are used for operators, but there's no matching operator for this sequence." << endl;
        }
        token = formToken(s);
        column += s.length();
    }
    else if (c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_') { // Se for um identificador/keyword
        // Esse codigo soh funciona se nenhuma keyword for prefixo de outra!!!
        int checkpoint = input.tellg();
        bool isKeyword = false;
        string s;

        tree.reset();
        for(input >> c; c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_' or c == ' ' ; input >> c) {
            if( not tree.canMove(c) ) {
                isKeyword = false;
                break;
            }
            tree.move(c);
            isKeyword = tree.isTerminal();
            s += c;
        }
        if (not isKeyword) { // Se nao eh keyword, entao eh um identificador
            input.seekg(checkpoint);
            s.clear();
            while (c = input.peek(), c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_' or c >= '0' and c <= '9') {
                s += c;
            }
        }
        token = formToken(s);
        column += s.length();
    }
    else if (c == '\'') {
        string s;
        bool foundEndingApostrophe = false;
        input >> c;
        s += c;
        while(input.peek() != EOF) {
            input >> c;
            s += c;
            if(c == '\'') {
                foundEndingApostrophe = true;
                break;
            }
        }
        if (input.eof() and not foundEndingApostrophe) { // Se chegou ao final do arquivo e nao achou o ' final
            ++errorCount;
            eout << "[SCANNER] ERROR at (lin " << line << ", col " << column << "): ";
            eout << "Missing ending apostrophe (') for char literal." << endl;
        }
        else if (s.length() < 3 and foundEndingApostrophe) { // Se era '' (sem caractere dentro)
            ++errorCount;
            eout << "[SCANNER] ERROR at (lin " << line << ", col " << column << "): ";
            eout << "Missing character inside char literal." << endl;
        }
        else if (s.length() > 3) { // Se tinha mais que um caractere entre os apostrofos
            ++errorCount;
            eout << "[SCANNER] ERROR at (lin " << line << ", col " << column << "): ";
            eout << "Multiple characters inside character literal. ";
            eout << "Make sure that there's only one character inside the char literal." << endl;
        }
        else if (unsigned(s[0]) > 0x7F) { // Se o caractere nao eh ASCII
            eout << "[SCANNER] WARNING at (lin " << line << ", col " << column + 1 << "): ";
            eout << "Non-ASCII character." << endl;
        }
        token = formToken(s);
        column += s.length();
    }
    else if (s == "\"") {
        input.ignore(1);
        getline(input, s, '\"');
        if (input.fail()) { // Se chegou ao final do arquivo e nao achou o " final
            ++errorCount;
            eout << "[SCANNER] ERROR at (lin " << line << ", col " << column << "): ";
            eout << "Missing ending quotation marks (\") for string literal." << endl;
            return 0;
        }
        else if (s.length() > MAX_STRING_SIZE) { // Se a string for maior que o maximo
            s = s.substr(0, MAX_STRING_SIZE);
            eout << "[SCANNER] WARNING at (lin " << line << ", col " << column << "): ";
            eout << "String bigger than the maximum permitted, which is " << MAX_STRING_SIZE << ". ";
            eout << "The string will be truncated to the maximum number of characters." << endl;
        }
        for (int i = 0; i < s.size(); ++i) { // Se tiver caracteres que nao sao ASCII
            if (s[i] > 0x7F) {
                eout << "[SCANNER] WARNING at (lin " << line << ", col " << column + i << "): ";
                eout << "Non-ASCII character." << endl;
            }
        }
        output << token("\"" + s + "\"", line, column) << endl;
        column += s.length() + 2;
    }
    else if (s >= "0" and s <= "9") { // Se for um literal numerico
        bool leadingZeros = true;
        while (c = input.peek(), c >= '0' and c <= '9') {
            if (not leadingZeros or not c == '0') {
                s += c;
                leadingZeros = false;
            }
            input.ignore(1);
        }
        if (input.peek() == '.') { // Se for float
            input >> c;
            s += c;
            while (c = input.peek(), c >= '0' and c <= '9') {
                s += c;
                input.ignore(1);
            }
        }
        output << token(s, line, column) << endl;
        column += s.length();
    }
    else {
        ++errorCount;
        eout << "[SCANNER] ERROR at (" << line << ", " << column << "): ";
        eout << "Invalid character. Make sure your file has only ASCII characters. ";
        eout << "You can use Unicode characters inside comments." << endl;
    }

    return pair(token, eout.str());
}
