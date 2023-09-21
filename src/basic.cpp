#include "../include/basic.h"
#include "../include/reader.h"


bool debug() {return 0;}

void debugprint(std::string input) {
    if (debug()) { std::cout << input << std::endl; }
}


void debugspot(std::string input, bool flag) {
    if (flag) { std::cout << input << std::endl; }
}
void debugspot(long input, bool flag) {
    if (flag) { std::cout << input << std::endl; }
}


bool printTokensCheck() {return 0;}

void printTokens(std::optional<std::string_view> token) {
    if (printTokensCheck()) { std::cout << "[" << *token << "]\n"; }
}