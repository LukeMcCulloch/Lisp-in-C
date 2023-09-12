
#include "../include/basic.h"
#include "../include/linenoise.hpp"
#include "../include/reader.h"


std::string read(std::string input) {
   Tokenizer tokenizer { input };
   while (auto token = tokenizer.next()) {
      std::cout << "[" << *token << "]\n";//dereference because it's optional
   }
   return input;
}

std::string eval(std::string input) {
   return input;
}

std::string print(std::string input) {
   return input;
}

std::string rep(std::string input) {
   auto ast    = read( input );
   auto result = eval(ast);
   return print(result);
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