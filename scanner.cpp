#include <bits/stdc++.h> // Mudar depois
#define MAX_STRING_SIZE 255

using namespace std;

unordered_set<string> operators = {
    "=", 
    "+",    "-",    "*",    "/",    "%",    "**",   "~",    "|",    "&",    "^", 
    "+=",   "-=",   "*=",   "/=",   "%=",   "**=",  "~=",   "|=",   "&=",   "^=",
    "==",   "!=", 
    "<",    ">",    "<=",   ">=", 
    "(",    ")",    "{",    "}",    ",",    ";"
};
unordered_set<string> operatorsCharacters = {
    "=", "+", "-", "*", "/", "%", "~", "|", "&", "^",
    "!", ">", "<", "(", ")", "{", "}", ",", ";"
};
unordered_set<string> keywords = {
    "or", "and",  "not",  "xor", "good", "evil",
    "draugr", "hobbit", "troll", "wyvern", "dragon", "istari", "tengwarr", "sindarin", "darkness",
    "The journey begins here", "Quest", "The quest continues", "You shall not pass!", "Given",
    "However when", "Otherwise", "go back to the abyss!" 
};

map<string, string> tokenString = {
    {"+", "TK_ADD"},            {"+=", "TK_ADD_ASSIGN"},
    {"-", "TK_SUB"},            {"-=", "TK_SUB_ASSING"},
    {"*", "TK_MULT"},           {"*=", "TK_MULT_ASSIGN"},
    {"/", "TK_DIV"},            {"/=", "TK_DIV_ASSIGN"},
    {"%", "TK_REST"},           {"%=", "TK_REST_ASSIGN"},
    {"**", "TK_POW"},           {"**=", "TK_POW_ASSIGN"},
    {"~", "TK_BITWISE_NOT"},    {"~=", "TK_BITWISE_NOT_ASSIGN"},
    {"|", "TK_BITWISE_OR"},     {"|=", "TK_BITWISE_OR_ASSIGN"},
    {"&", "TK_BITWISE_AND"},    {"&=", "TK_BITWISE_AND_ASSIGN"},
    {"^", "TK_BITWISE_XOR"},    {"^=", "TK_BITWISE_XOR_ASSIGN"},

    {"==", "TK_EQUAL"},
    {"!=", "TK_DIFFERENT"},
    {"<", "TK_LESS"},
    {"<=", "TK_LESS_EQUAL"},
    {">", "TK_GREATER"},
    {">=", "TK_GREATER_EQUAL"},
    
    {"=", "TK_ASSIGN"},

    {"(", "TK_OPEN_PARENTHESIS"},   {")", "TK_CLOSE_PARENTHESIS"}, 
    {"{", "TK_OPEN_BRACE"},         {"}", "TK_CLOSE_BRACE"},
    {";", "TK_SEMICOLON"},
    {",", "TK_COMMA"},

    {"or", "TK_OR"},
    {"and", "TK_AND"},
    {"not", "TK_NOT"},
    {"xor", "TK_XOR"},

    {"good", "TK_TRUE"},            {"evil", "TK_FALSE"},

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

/*class node {
    bool mark;
    vector<unsigned> child;
}

class trie {
    vector<node> vec;
    unsigned currentState = 0;

    enum trie_state { 
    public:
    void insert(string s) {

    }
    


    private:

}*/


enum exception_type {
    TOO_MANY_ARGUMENTS, MISSING_INPUT_FILE, WRONG_INPUT_FILE_EXTENSION,
};
typedef exception_type file_exception;


string token(string s, unsigned line, unsigned column);
string token(string s, unsigned line, unsigned column, string valor);
string getExtension(string s);


int main(int argc, char *argv[]) {
    int errorCounter = 0;

    ifstream input;
    ofstream output;
    input.exceptions(ifstream::failbit | ifstream::badbit);     // Se houver qualquer erro na abertura,
    output.exceptions(ifstream::failbit | ifstream::badbit);    // leitura ou escrita, dá throw.

    try {
        if(argc > 2) throw TOO_MANY_ARGUMENTS;
        if(argc < 2) throw MISSING_INPUT_FILE;
        if(argc == 2) {
            string inputFileName = argv[1];
            if(getExtension(inputFileName) != ".mepp") throw WRONG_INPUT_FILE_EXTENSION;
            input.open(string(argv[1]));
            string outputFileName = (inputFileName.begin(), inputFileName.end()-strlen(".mepp"));
            output.open(outputFileName, ofstream::out | ofstream::trunc);
        }
    }
    catch(const file_exception &error) {
        if(error == TOO_MANY_ARGUMENTS)
            cout << "[SCANNER] ERROR: ";
            cout << "Too many input files. Make sure to write only the input file name, with extension .mepp." << endl;
        else if(error == MISSING_INPUT_FILE)
            cout << "[SCANNER] ERROR: ";
            cout << "Missing input file. Make sure to write only the input file name, with extension .mepp." << endl;
        else if(error == WRONG_INPUT_FILE_EXTENSION)
            cout << "[SCANNER] ERROR: ";
            cout << "Input file is not a .mepp file." << endl;
        return 0;
    }
    catch(const ifstream::failure &error) {
        cout << "[SCANNER] ERROR: ";
        cout << "Couldn't open the input/output files. ";
        cout << "Make sure they're not being used by other process." << endl;
        return 0;
    }


    unordered_set<char> whitespace = {' ', '\f', '\r', '\t', '\v'}; // Set com todos os whitespaces. \n tratado separadamente.
    unordered_set<char> idChars = { // Set com todos os caracteres permitidos para os identificadores.
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
        '_'
    }

    int line = 1, column = 1;
    while(!input.eof()) try {
        char c;
        string s = input.peek();

        while(whitespace.count(s) == 1) {
            input.ignore(1);
            ++column;
            s = input.peek();
        }

        if(s == "\n" or s == "#") {
            input.ignore(numeric_limits<streamsize>::max(), "\n");
            ++line;
            column = 1;
        }
        else if(operatorsCharacters.count(s) == 1) {    // Se for um dos caracteres das transicoes de S5 e S6
            while(operators.count( s+char(input.peek()) ) == 1) {   // Esse while pega o proximo caractere se, ao adicionar ele,
                input >> c; s.append(1, c);
            }

            if(operators.count(s) == 0) {
                ++errorCounter;
                cout << "[SCANNER] ERROR at (" << line << ", " << column << "): ";
                cout << "Unknown operator. This happens when there's a sequence of characters "; 
                cout << "used for operators, but there's no matching operator for this sequence."
            }
            output << token(s, line, column);
            column += s.length();
        }
        else if(s == "\'") {
            input.ignore(1);
            getline(input, s, '\'');
            if(input.fail()) { // Se chegou ao final do arquivo e nao achou o ' final
                ++errorCounter;
                cout << "[SCANNER] ERROR at (" << line << ", " << column << "): ";
                cout << "Missing ending apostrophe (') for char literal."; << endl;
                return 0;
            }
            else if(s.length() == 0) { // Se era '' (sem caractere dentro)
                ++errorCounter;
                cout << "[SCANNER] ERROR at (" << line << ", " << column << "): ";
                cout << "Missing character inside char literal." << endl; 
            }
            else {
                if(s.length > 1) { // Se tinha mais que um caractere entre os apostrofos
                    ++errorCounter;
                    cout << "[SCANNER] ERROR at (" << line << ", " << column << "): ";
                    cout << "Multiple characters inside character literal. ";
                    cout << "Make sure that there's only one character inside the char literal." << endl;
                }
                if(s[0] > 0x7F) { // Se o caractere nao eh ASCII
                    cout << "[SCANNER] WARNING at (" << line << ", " << column+1 << "): ";
                    cout << "Non-ASCII character." << endl; 
                }
            }
            output << token(s, line, column);
            column += s.length() + 2;
        }
        else if(s == "\"") {
            input.ignore(1);
            getline(input, s, '\"');
            if(input.fail()) { // Se chegou ao final do arquivo e nao achou o " final
                ++errorCounter;
                cout << "[SCANNER] ERROR at (" << line << ", " << column << "): ";
                cout << "Missing ending quotation marks (\") for string literal."; << endl;
                return 0;
            }
            else if(s.length() > STRING_MAX_SIZE) { // Se a string for maior que o maximo
                s = s.substr(0, STRING_MAX_SIZE);
                cout << "[SCANNER] WARNING at (" << line << ", " << column << "): ";
                cout << "String bigger than the maximum permitted, which is " << MAX_STRING_SIZE << ". "; 
                cout << "The string will be truncated to the maximum number of characters." << endl; 
            }
            for(int i = 0; i<s.size(); ++i) { // Se tiver caracteres que nao sao ASCII
                if(s[i] > 0x7F) { 
                    cout << "[SCANNER] WARNING at (" << line << ", " << column+i << "): ";
                    cout << "Non-ASCII character." << endl; 
                }
            }
            output << token(s, line, column);
            column += s.length() + 2;
        }
	else if(s >= "0" and s <= "9") { // Se for um literal numerico
            while(c = input.peek(), c >= '0' and c <= '9') {
                s.append(1, c);
                input.ignore(1);
            }
            if(input.peek() == ".") { // Se for float
                input >> c;
                s.append(1, c);
                while(c = input.peek(), c >= '0' and c <= '9') {
                    s.append(1, c);
                    input.ignore(1);
                }
            }
            output << token(s, line, column);
            column += s.length();
	}
        else if(s >= 'a' and s <= 'z' or s >= 'A' and s <= 'Z' or s == '_') { // Se for um identificador/keyword
            // Esse codigo soh funciona se nenhuma keyword for prefixo de outra!!!
            int beginning = input.tellg();
            bool isKeyword = false;
            for(auto it = keywords.begin(); it != keywords.end(); ++it) { // Teste se eh uma das keywords
                input.seekg(beginning);
                isKeyword = true;
                s = *it;
                for(int i = 0; i < s.length(); ++i) {
                    input >> c;
                    if(c != s[i]) {
                        isKeyword = false;
                        break;
                    }
                }
                // Se for detectada uma keyword, mas na real eh o prefixo de um identificador (ex: Questo),
                // desabilita a flag isKeyword.
                if(isKeyword) {
                    c = s.peek();
                    if(not (c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_' or c >= '0' and c <= '9') )
                        break;
                    else
                        isKeyword = false;
                }

            }
            if(not isKeyword) { // Se nao eh keyword, entao eh um identificador
                s.clear();
                input.seekg(beginning);
                while(c = input.peek(), c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_' or c >= '0' and c <= '9') {
                    s.append(1, c);
                }
            }
            output << token(s, line, column);
            column += s.length();
        }
        else {
            ++errorCounter;
            cout << "[SCANNER] ERROR at (" << line << ", " << column << "): ";
            cout << "Invalid character. Make sure your file has only ASCII characters. ";
            cout << "You can use Unicode characters inside comments." << endl; 
        }

    }
    catch(const ifstream::exception &error) {
        cout << "[SCANNER] ERROR: ";
        cout << "There was an error during the manipulation of the files: " << error.what() << endl;
    }

    return 0;
}



string getExtension(string s)
    return string(find(s.begin(), s.end(), '.'), s.end());

string token(string s, unsigned line, unsigned column) {
    return "";
}
string token(string s, unsigned line, unsigned column, string valor) {
    return "";
}
