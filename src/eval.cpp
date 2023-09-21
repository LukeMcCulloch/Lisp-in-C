
#include "../include/basic.h"
#include "../include/linenoise.hpp"
#include "../include/reader.h"
#include "../include/printer.h"

Value* READ(std::string input) {
    debugprint("read");
    return read_str(input);
}

Value* EVAL(Value *input) {
    debugprint("EVAL");
   return input;
}

std::string PRINT(Value* input) {
    debugprint("PRINT");
   return pr_str(input);
}



std::string rep(std::string input) {
    debugprint("rep");
   auto ast    = READ(input);
   auto result = EVAL(ast);
   return PRINT(result);
}

int main() {
   //debugprint("Luke's CppLispy");
   const auto history_path = "history.txt";
   linenoise::LoadHistory(history_path);

   std::string input;
   for (;;) {
      auto quit = linenoise::Readline("user> ", input);
      if (quit) break;
      std::cout << rep(input) << std::endl;
      // Add text to history
      linenoise::AddHistory(input.c_str());
   }
   linenoise::SaveHistory(history_path);
   
   return 0;
}