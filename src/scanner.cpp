#include "scanner.hpp"

namespace std {
    string Scanner::error(const string &msg, const int lin , const int col) {
        ++errorCount;
        stringstream s;
        s << "[SCANNER] ERROR at (lin " << lin << ", col " << col << "): " << msg;
        return s.str();
    }

    string Scanner::warning(const string &msg, const int lin, const int col) {
        stringstream s;
        s << "[SCANNER] WARNING at (lin " << lin << ", col " << col << "): " << msg << endl;
        return s.str();
    }

    string Scanner::formToken(const string &type, const string &value = "") {
        string s = value;
        for(int i = 0; i<s.length(); ++i) {
            if (s[i] == ',') {
                s.replace(i, 1, "\\,");
                ++i;
            }
        }
        return type + "," + s + "," + to_string(line) + "," + to_string(column);
    }

    void Scanner::jumpToNextToken() {
        char c;
        while (c = input.peek(), not input.eof() and (isspace(c) != 0 or c == '#')) {
            if (c == '\n' or c == '#') {
                input.ignore(numeric_limits<streamsize>::max(), '\n');
                ++line;
                column = 1;
            }
            else {
                input.ignore(1);
                ++column;
            }
        }
    }

    Scanner::Scanner() {
        input.exceptions(ifstream::failbit | ifstream::badbit); // Se houver qualquer erro na abertura
                                                                // ou leitura, da throw ifstream::failure e deixa para a main tratar
    }

    Scanner::Scanner(const string &fileName) {
        input.exceptions(ifstream::failbit | ifstream::badbit); // Se houver qualquer erro na abertura
                                                                // ou leitura, da throw ifstream::failure e deixa para a main tratar
        open(fileName);
    }

    void Scanner::open(const string &fileName) {
        if(input.is_open()) {
            input.close();
        }
        input.open(fileName);
        errorCount = 0;
        line = column = 1;
    }

    bool Scanner::eof() { 
        jumpToNextToken();
        return input.eof();
    }

    pair<string,string> Scanner::getToken() {
        if(this->eof()) {
            throw out_of_range("Scanner exception - getToken was called when EOF was reached already.");
        }

        string token;       // Onde o token eh escrito
        stringstream eout;  // Error output, assim como cout eh character output :)
                            // Os erros sao escritos aqui, para serem entregues na saida do metodo.

        jumpToNextToken();

        char c = input.peek();

        if (delimiters.count(c) == 1) {
            input.ignore(1);
            token = formToken(sequence2tokenType.at(string(1, c)));
            ++column;
        }
        else if (operatorsCharacters.count(c) == 1) { // Se for um dos caracteres das transicoes de S2 e S3
            input.ignore(1);
            string sequence(1, c);
            while (operators.count(sequence + char(input.peek())) == 1) {// Esse while pega o proximo caractere se, ao
                c = input.get();                                         // adicionar ele, eh formado um outro operator
                sequence += c;
            }

            if (sequence2tokenType.count(sequence) == 0) {
                eout << error("Unknown operator \"" + sequence + "\".", line, column);
                token = formToken("TK_UNKNOWN_OPERATOR");
            }
            else {
                token = formToken(sequence2tokenType.at(sequence));
            }
            column += sequence.length();
        }
        else if (c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_') { // Se for um identificador/keyword
            // Esse codigo soh funciona se nenhuma keyword for prefixo de outra keyword!!!
            string sequence;
            int checkpoint = input.tellg();

            bool isKeyword = false;
            tree.reset();
            while(tree.canMove(char(input.peek()))) {
                c = input.get();
                tree.move(c);
                sequence += c;
            }
            // Se parou em um nodo terminal, e o que foi lido nao for o prefixo de um identificador, entao eh uma keyword mesmo
            c = input.peek();
            if(tree.isTerminal() and not (c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_' or c >= '0' and c <= '9')) {
                token = formToken(sequence2tokenType.at(sequence));
                column += sequence.length();
            }
            else { // Se nao eh keyword, entao eh um identificador
                input.seekg(checkpoint); // Volta para o comeco da leitura
                sequence.clear();
                bool tooBig = false;
                while (c = input.peek(), c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_' or c >= '0' and c <= '9') {
                    if(sequence.length() > MAX_IDENTIFIER_SIZE) {
                        tooBig = true;
                    }
                    sequence += c;
                    input.ignore(1);
                }
                if(tooBig) {
                    eout << error("Identifier bigger than the maximum, which is " + to_string(MAX_IDENTIFIER_SIZE) + ".", line, column);
                }
                token = formToken("TK_ID", sequence.substr(0, min(MAX_IDENTIFIER_SIZE, int(sequence.length()))));
                column += sequence.length();
            }
        }
        else if (c == '\'') { // Literal de char
            int checkpoint = input.tellg();
            bool foundEndApostrophe = false;
            string sequence;
            sequence += char(input.get());
            while(not input.eof() and input.peek() != '\n') {
                c = input.get();
                sequence += c;
                if(c == '\'') {
                    foundEndApostrophe = true;
                    break;
                }
            }
            /*  Tipos de char literals que podem ser formados:
            *        '' -> Erro. Falta o caractere dentro.
            *       'a' -> Certo.
            *   'ab...' -> Erro. Mais de um caractere.
            *         ' -> Erro. Falta o caractere e o apostrofo final.
            *        'a -> Erro. Falta o apostrofo final. Considera 'a como 'a' para continuar a compilacao.
            *    'ab... -> Erro. Falta o apostrofo final. Considera 'a como 'a' para continuar a compilacao, e o resto (b...) volta para o arquivo.
            */
            char content;
            if (foundEndApostrophe) {
                if (sequence.length() == 2) { // ''
                    eout << error("Missing character inside char literal.", line, column);
                    content = '\0';
                }
                else if (sequence.length() > 3) { // 'ab...'
                    eout << error("Multiple characters inside character literal.", line, column);
                    content = sequence[1];
                }
                else { // 'a'
                    content = sequence[1];
                }
            }
            else if (not foundEndApostrophe) {
                if (sequence.length() == 1) { // '
                    eout << error("Missing ending apostrophe (') and char for char literal.", line, column);
                    content = '\0';
                }
                else {
                    input.seekg(checkpoint); // 'a ou 'ab...
                    sequence += char(input.get());
                    sequence += char(input.get());
                    eout << error(string("Missing ending apostrophe (') for char literal, after ") + sequence + ".", line, column);
                    content = sequence[1];
                }
            }
            token = formToken("TK_CHAR_LITERAL", string(1, content));
            column += sequence.length();
        }
        else if (c == '\"') { // Literal de string
            string content;
            string sequence;
            bool foundEndQuotes = false;
            c = input.get();
            sequence += c;
            while(not input.eof() and input.peek() != '\n') {
                c = input.get();
                sequence += c;
                if(c == '\"') {
                    foundEndQuotes = true;
                    break;
                }
            }
            /*  Tipos de string literals que podem ser formados:
            *        "" -> Certo.
            *   "ab..." -> Certo.
            *         " -> Erro. Falta as aspas finais.
            *    "ab... -> Erro. Falta as aspas finais.
            */
            if (not foundEndQuotes) { // " ou "ab...
                eout << error("Missing ending quotes (\") for string literal.", line, column);
                content = sequence.substr(1);
            }
            else {
                content = sequence.substr(1, sequence.length()-2);
            }
            token = formToken("TK_STRING_LITERAL", content);
            column += sequence.length();
        }
        else if (c >= '0' and c <= '9' or c == '.') { // Se for um literal numerico
            string sequence;
            bool isFloat = false;
            while (c = input.peek(), c >= '0' and c <= '9') {
                sequence += c;
                input.ignore(1);
            }
            if (input.peek() == '.') { // Se for float
                isFloat = true;
                c = input.get();
                sequence += c;
                while (c = input.peek(), c >= '0' and c <= '9') {
                    sequence += c;
                    input.ignore(1);
                }
            }
            string literal = sequence;
            if (c = input.peek(), c>='a' and c<='z' or c>='A' and c<='Z' or c=='_') {
                eout << error("Numeric literals can only have digits and one period. If it should be an identifier, make "
                            "sure it starts with an alphabet letter or underline (_).", line, column+sequence.length());
                while (c = input.peek(), c>='a' and c<='z' or c>='A' and c<='Z' or c=='_' or c>='0' and c<='9' or c=='.') {
                    sequence += c;
                    input.ignore(1);
                }
            }
            if (literal.find_first_not_of('0') == string::npos) { // Se for soh 0, como 000, 000000 ou 0
                literal.erase(0, literal.length()-1); // Deixa apenas um 0;
            }
            else {
                literal.erase(0, literal.find_first_not_of('0')); // Tira os 0 a esquerda
            }
            if(isFloat) {
                token = formToken("TK_FLOAT_LITERAL", literal);
            }
            else {
                token = formToken("TK_INT_LITERAL", literal);
            }
            column += sequence.length();
        }
        else if (alphabet.count(c) == 0) { // Caracteres fora do alfabeto
            string sequence;
            while (not input.eof()) {
                c = input.peek();
                if (alphabet.count(c) == 1) {
                    break;
                }
                else {
                    sequence += c;
                    input.ignore();
                }
            }
            eout << error("Invalid character sequence " + sequence + ". Make sure your file has only ASCII characters."
                          , line, column);
            token = "";
            column += sequence.length();
        }
        else { // Caractere nao reconhecido
            eout << error("Couldn't identify the character. This is probably due to an implementation error.", line, column);
            input.ignore(1);
            ++column;
        }

        return pair(token, eout.str());
    }
}