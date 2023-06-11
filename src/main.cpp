#include <iostream>
#include <string> 
#include <fstream>
#include <utility>
#include "scanner.hpp"
#define MAX_STRING_SIZE 255

using namespace std;

string getExtension(string &s) {
    if (s.find_last_of('.') == string::npos) {
        return "";
    }
    else {
        return s.substr(s.find_last_of('.'), string::npos);
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << "ERROR: ";
        cout << "Missing input file. Make sure to write only the input file name, with extension .mepp." << endl;
        return 0;
    }
    if(argc > 2) {
        cout << "ERROR: ";
        cout << "Too many input files. Make sure to write only the input file name, with extension .mepp." << endl;
        return 0;
    }

    string inputFileName = argv[1];
    if(getExtension(inputFileName) != ".mepp") {
        cout << "ERROR: ";
        cout << "Input file is not a .mepp file." << endl;
        return 0;
    }
    // Testar se o arquivo existe, depois criar o scanner
   
    Scanner scanner;

    try {
        scanner.open(inputFileName);
    }
    catch(const ifstream::failure &error) {
        cout << "ERROR: Could not open the file." << endl;
        cout << error.what();
        return 0;
    }

    while(not scanner.eof()) {
        scannerOutput token = scanner.getToken();
        cout << "Token: " << token.token << endl;
        cout << "Sequence: " << token.sequence << endl;
        if(token.error != "") {
            cout << "   Error: " << token.error << endl;
        }
    }

    return 0;
}