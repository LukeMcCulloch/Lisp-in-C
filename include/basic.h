#ifndef basic_h
#define basic_h

#include <iostream>
#include <string>
#include <optional>

bool debug();

void debugprint(std::string input);

bool printTokensCheck();
void printTokens(std::optional<std::string_view> input);

#endif /* __basic_h */