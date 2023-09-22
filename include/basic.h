#ifndef basic_h
#define basic_h

#include <iostream>
#include <string>
#include <optional>

bool debug();
void debugprint(std::string input);
void debugprint(long input);

bool printTokensCheck();
void printTokens(std::optional<std::string_view> input);

void debugspot(std::string input, bool flag);
void debugspot(long input, bool flag);

#endif /* __basic_h */