#include <algorithm>
#include <string>
#include <iostream>
#include <regex>

using namespace std;

int main() {
    // const std::string needFind = "\r\n";
    // std::string buf = "123456789\r\n";
    // auto end = search(buf.begin(), buf.end(), needFind.begin(), needFind.end());
    // std::string line(buf.begin(), end);
    // cout << line.size() << endl;

    // string line = "GET /562f25980001b1b106000338.jpg HTTP/1.1";
    string line = "once a bog";
    // regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    regex patten("^once");
    smatch subMatch;

    if (regex_match(line, subMatch, patten)) {
        cout << subMatch[1] << endl;
    } else {
        cout << "no" << endl;
    }
}