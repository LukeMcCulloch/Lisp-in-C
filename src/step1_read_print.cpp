
#include "../include/basic.h"
#include "../include/linenoise.hpp"
#include "../include/reader.h"
#include "../include/printer.h"


// std::string read(std::string input) {
//    Tokenizer tokenizer { input };
//    while (auto token = tokenizer.next()) {
//       std::cout << "[" << *token << "]\n";//dereference because it's optional
//    }
//    return input;
// }


Value* READ(std::string input) {
   return read_str(input);
}

Value* EVAL(Value *input) {
   return input;
}

std::string PRINT(Value* input) {
   return pr_str(input);
}

// std::string rep(std::string input) {
//    auto ast    = read( input );
//    auto result = eval(ast);
//    return print(result);
// }

std::string rep(std::string input) {
   auto ast    = READ(input);
   auto result = EVAL(ast);
   return PRINT(result);
}

int main() {

   const auto history_path = "history.txt";
   // Load history
   linenoise::LoadHistory(history_path);

   std::string input;
   for (;;) {
      auto quit = linenoise::Readline("user> ", input);
      
      //std::cout << "user> ";
      if (quit) break;
      //if (!std::getline(std::cin, input)) break;
      
      std::cout << rep(input) << std::endl;

      // Add text to history
      linenoise::AddHistory(input.c_str());
         
   }

   // Save history
   linenoise::SaveHistory(history_path);
   
   return 0;
}