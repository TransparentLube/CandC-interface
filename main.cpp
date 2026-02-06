#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {

    #ifdef _WIN32
    string p = "C:\\";
    ofstream out("output.txt");
    #endif

    #ifdef __linux__
    string p = "/home/";
    ofstream out("output.txt");
    #endif
    //testing fstream
    out << "This is a test." << endl;
    out.close();

}