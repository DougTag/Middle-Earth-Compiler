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

    void Scanner::jumpWhitespace() {
        char c;
        while (c = input.peek(), input.peek() != EOF and (isspace(c) != 0 or c == '#')) {
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

    Scanner::Scanner() {}

    Scanner::Scanner(const string &fileName) {
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
        jumpWhitespace();
        return input.eof() or input.peek() == EOF;
    }

    scannerOutput Scanner::getToken() {
        if(this->eof()) {
            throw out_of_range("Scanner exception - getToken was called when EOF was reached already.");
        }

        scannerOutput scanout;
        string token;       // Onde o token eh escrito
        stringstream eout;  // Error output, assim como cout eh character output :)
                            // Os erros sao escritos aqui, para serem entregues na saida do metodo.

        jumpWhitespace();

        char c;

        while(alphabet.count(input.peek()) == 0) { 
            string sequence;
            while (input.peek() != EOF) {
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
            column += sequence.length();
            jumpWhitespace();
        }

        scanout.line = line;
        scanout.column = column;

        string sequence;
        c = input.peek();

        if (delimiters.count(c) == 1) {
            input.ignore(1);
            scanout.token = sequence2tokenType.at(string(1, c));
            scanout.sequence = c;
            ++column;
        }
        else if (operatorsCharacters.count(c) == 1) { // Se for um dos caracteres das transicoes de S2 e S3
            input.ignore(1);
            sequence = c;
            // Esse while pega o proximo caractere se, ao adicionar ele, eh formado um outro operator
            while (input.peek() != EOF and operators.count(sequence + char(input.peek())) == 1) {
                c = input.get();
                sequence += c;
            }

            if (sequence2tokenType.count(sequence) == 0) {
                eout << error("Unknown operator \"" + sequence + "\".", line, column);
                scanout.token = "";
            }
            else {
                scanout.token = sequence2tokenType.at(sequence);
            }
            scanout.sequence = sequence;
            column += sequence.length();
        }
        else if (c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_') { // Se for um identificador/keyword
            // Esse codigo soh funciona se nenhuma keyword for prefixo de outra keyword!!!
            int checkpoint = input.tellg();

            bool isKeyword = false;
            tree.reset();
            while(input.peek() != EOF and tree.canMove(char(input.peek()))) {
                c = input.get();
                tree.move(c);
                sequence += c;
            }
            // Se parou em um nodo terminal, e o que foi lido nao for o prefixo de um identificador, entao eh uma keyword mesmo
            c = input.peek();
            if(tree.isTerminal() and not (c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_' or c >= '0' and c <= '9')) {
                scanout.token = sequence2tokenType.at(sequence);
                scanout.sequence = sequence;
            }
            else { // Se nao eh keyword, entao eh um identificador
                input.seekg(checkpoint); // Volta para o comeco da leitura
                sequence.clear();
                bool tooBig = false;
                while (c = input.peek(), input.peek() != EOF and c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_' or c >= '0' and c <= '9') {
                    sequence += c;
                    input.ignore(1);
                    if(sequence.length() > MAX_IDENTIFIER_SIZE) {
                        tooBig = true;
                    }
                }
                if(tooBig) {
                    eout << error("Identifier bigger than the maximum, which is " + to_string(MAX_IDENTIFIER_SIZE) + ".", line, column);
                }
                scanout.token = "TK_ID";
                scanout.sequence = sequence.substr(0, min(MAX_IDENTIFIER_SIZE, int(sequence.length())));
            }
            column += sequence.length();
        }
        else if (c == '\'') { // Literal de char
            int checkpoint = input.tellg();
            bool foundEndApostrophe = false;
            sequence += char(input.get());
            while(input.peek() != EOF and input.peek() != '\n') {
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
            if (foundEndApostrophe) {
                if (sequence.length() == 2) { // ''
                    eout << error("Missing character inside char literal.", line, column);
                }
                else if (sequence.length() > 3) { // 'ab...'
                    eout << error("Multiple characters inside character literal.", line, column);
                }
            }
            else if (not foundEndApostrophe) {
                if (sequence.length() == 1) { // '
                    eout << error("Missing ending apostrophe (') and char for char literal.", line, column);
                }
                else {
                    input.seekg(checkpoint); // 'a ou 'ab...
                    sequence.clear();
                    sequence += char(input.get());
                    sequence += char(input.get());
                    eout << error(string("Missing ending apostrophe (') for char literal, after ") + sequence + ".", line, column);
                }
            }
            scanout.token = "TK_CHAR_LITERAL";
            scanout.sequence = sequence;
            column += sequence.length();
        }
        else if (c == '\"') { // Literal de string
            bool foundEndQuotes = false;
            //c = input.get();
            input >> c;
            sequence += c;
            while(input.peek() != EOF and input.peek() != '\n') {
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
            }
            scanout.token = "TK_STRING_LITERAL";
            scanout.sequence = sequence;
            column += sequence.length();
        }
        else if (c >= '0' and c <= '9' or c == '.') { // Se for um literal numerico
            bool isFloat = false;
            while (c = input.peek(), input.peek() != EOF and c >= '0' and c <= '9') {
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
            if (c = input.peek(), input.peek() != EOF and c>='a' and c<='z' or c>='A' and c<='Z' or c=='_') {
                eout << error("Numeric literals can only have digits and one period. If it should be an identifier, make "
                            "sure it starts with an alphabet letter or underline (_).", line, column+sequence.length());
                while (c = input.peek(), input.peek() != EOF and c>='a' and c<='z' or c>='A' and c<='Z' or c=='_' or c>='0' and c<='9' or c=='.') {
                    sequence += c;
                    input.ignore(1);
                }
            }
            if(isFloat) {
                scanout.token = "TK_FLOAT_LITERAL";
            }
            else {
                scanout.token = "TK_INT_LITERAL";
            }
            scanout.sequence = sequence;
            column += sequence.length();
        }
        else { // Caractere nao reconhecido. Esse else soh eh executado se houve uma falha na implementacao do scanner
            eout << error("Couldn't identify the character. This is probably due to an implementation error.", line, column);
            token = "";
            input.ignore(1);
            ++column;
        }

        scanout.error = eout.str();

        return scanout;
    }
}