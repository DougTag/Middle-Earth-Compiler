#ifndef SCANNER_HEADER_GUARD
#define SCANNER_HEADER_GUARD

#include <fstream> 
#include <string> 
#include <unordered_set> 
#include <utility> // Contem o tipo pair<T,T>
#include <sstream> // Contem stringstream
#include <limits>
#include <tuple>
#include "trie.hpp"

#define MAX_IDENTIFIER_SIZE 31

namespace std {
    typedef struct scannerOutput {
        int line;
        int column;
        string token;
        string sequence;
        string error;
    } scannerOutput;

    class Scanner {
    private:
        ifstream input;
        int errorCount = 0, line = 1, column = 1;
        unordered_set<char> alphabet = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            '_', '!', '#', '%', '&', '*', '(', ')', '-', '+', '=', '{', '}',
            ';', '\"','\'','/', '^', '~', '|', ',', '.', '<', '>',
            '\n','\t','\v','\f','\r',' '
        };
        unordered_set<char> delimiters = { // Set com todos os delimiters
            '(', ')', '{', '}', ',', ';', '!'
        };
        unordered_set<string> operators = { // Set com todos os operators
            "+", "-", "*", "/", "%", "**", "~", "|", "&", "^",
            "+=", "-=", "*=", "/=", "%=", "**=", "~=", "|=", "&=", "^=", "=",
            "==", "<>", "<", ">", "<=", ">=",
        };
        unordered_set<char> operatorsCharacters = { // Set com os caracteres permitidos para os operators
            '=', '+', '-', '*', '/', '%', '~', '|', '&', '^',
            '>', '<'
        };
        unordered_set<string> keywords = { // Set com as palavras/frases reservadas
            "or", "and", "not", "xor", "good", "evil",
            "draugr", "hobbit", "elf", "troll", "wyvern", "dragon", "istari", "tengwarr", "sindarin", "darkness",
            "The journey begins here", "Quest", "The quest continues", "You shall not pass", "Given",
            "However when", "Otherwise", "Go back to the abyss", "Prologue"
        };
        unordered_map<string, string> sequence2tokenType = { // Map da cadeia para o respectivo tipo de token
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
            {"<>", "TK_DIFFERENT"},
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
            {"!", "TK_EXCLAMATION"},

            {"or", "TK_OR"},
            {"and", "TK_AND"},
            {"not", "TK_NOT"},
            {"xor", "TK_XOR"},

            {"good", "TK_TRUE"},
            {"evil", "TK_FALSE"},

            {"draugr", "TK_UNSIGNED"},
            {"hobbit", "TK_SHORT"},
            {"elf", "TK_INT"},
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
            {"However when", "TK_ELSE_IF"},
            {"Otherwise", "TK_ELSE"},
            {"Go back to the abyss", "TK_RETURN"},
            {"Prologue", "TK_FUNCTION"}
        };
        Trie tree = {
            "or", "and", "not", "xor", "good", "evil",
            "draugr", "hobbit", "elf", "troll", "wyvern", "dragon", "istari", "tengwarr", "sindarin", "darkness",
            "The journey begins here", "Quest", "The quest continues", "You shall not pass", "Given",
            "However when", "Otherwise", "Go back to the abyss", "Prologue"
        };

        string error(const string &msg, const int lin, const int col);
        string warning(const string &msg, const int lin, const int col);
        void jumpWhitespace();

    public:
        Scanner();
        Scanner(const string &fileName);
        void open(const string &fileName);
        bool eof();
        scannerOutput getToken();
    };

}

#endif // SCANNER_HEADER_GUARD