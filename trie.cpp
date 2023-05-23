#include "trie.h"

namespace std {
    class Trie::Node {
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

    Trie::Trie() {
        curNode = 0;
        v.push_back(Node());
    }
    Trie::Trie(initializer_list<string> lst) {
        *this = Trie();
        for(auto s : lst){ this->insert(s); }
    }

    void Trie::insert(const string& s) {
        int current = 0;
        for(int i = 0; i<s.length(); ++i) {
            if(not v[current].hasChild(s[i])) {
                v.push_back(Node());
                v[current].insert(s[i], v.size());
            }
            current = v[current][s[i]];
            if(i == s.length()-1) {
                v[current].setTerminal(true);
            }
        }
    }
    void Trie::reset() { 
        curNode = 0; 
    }

    bool Trie::canMove(char c) {
        return v[curNode].hasChild(c);
    }

    void Trie::move(char c) {
        curNode = v[curNode][c];
    }
    
    bool Trie::isTerminal() {
        return v[curNode].getTerminal();
    }
}
