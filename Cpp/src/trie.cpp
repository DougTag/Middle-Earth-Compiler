#include "trie.hpp"

namespace std 
{
    Trie::Trie() {
        curNode = 0;
        v.push_back(Node()); // Raíz
    }
    Trie::Trie(initializer_list<string> lst) : Trie::Trie() {
        v.push_back(Node()); // Raíz
        for(auto s : lst) { this->insert(s); }
    }

    void Trie::insert(const string& s) {
        int current = 0;
        for(int i = 0; i<s.length(); ++i) {
            if(not v[current].hasChild(s[i])) {
                v.push_back(Node());
                v[current].insert(s[i], v.size()-1);
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
