/*  Essa implementacao da Trie nao eh das melhores,
    mas foi feita para funcionar com o compilador.
    Ela funciona para esse proposito, e como a dis-
    ciplina nao cobra uma implementacao de uma trie,
    nao vou me esforcar em fazer algo super bom.
*/

#ifndef TRIE_HEADER_GUARD
#define TRIE_HEADER_GUARD
#include <string>
#include <vector>
#include <unordered_map>

namespace std {
    class Trie { // Apenas para UTF-8. Sem funcao de deletar palavras.
    private:
        class Node {
            unordered_map<char, int> children;
            bool terminal = false;

            public:
            Node(bool isTerminal = false) { terminal = isTerminal; }
            bool getTerminal() { return terminal; }
            void setTerminal(bool b) { terminal = b; }
            void insert(char c, int i) { children[c] = i; }
            bool hasChild(char c) { return children.count(c) > 0; }
            int operator[](char c) { return children.at(c); }
        };

        vector<Node> v;
        int curNode;
        
    public:
        Trie();
        Trie(initializer_list<string> lst);
        void insert(const string& s); // Insere uma string na trie.
        void reset(); // Volta para a raiz.
        bool canMove(char c); // Checa se eh possivel mover.
        void move(char c); // Move para o proximo nodo. Dispara exception::out_of_range se mover para um nodo inexistente.
        bool isTerminal();
    };
} // namespace std

#endif // TRIE_HEADER_GUARD