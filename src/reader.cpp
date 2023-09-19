#include "../include/reader.h"
#include "../include/basic.h"


// std::vector<std::string_view> tokenize(std::string &input) {
//     Tokenizer tokenizer { input };
//     //Reader reader { input };
//     std::vector<std::string_view> vector;
//     while (auto token = tokenizer.next()) {
//         vector.push_back(token);
//     }
//     return vector;
// }

std::vector<std::string_view> tokenize(std::string &input) {
    Tokenizer tokenizer { input };
    std::vector<std::string_view> vector;
    while (auto token = tokenizer.next()) {
        //std::cout << "[" << *token << "]\n";//dereference because it's optional
        printTokens(*token);
        vector.push_back(*token);
    }
    return vector;
}


// Value *read_str(std::string &input) {
//     //TODO: tokenize, parse into AST, return the AST
// }

Value *read_str(std::string &input) {
    auto tokens = tokenize(input);
    Reader reader { tokens };
    return read_form(reader);
}

Value *read_form(Reader &reader) {
    auto token = reader.peek();

    if (!token) return nullptr;//garbage collection to be done
    
    switch (token.value()[0]) {
        case '(':
            return read_list(reader);
        default:
            return read_atom(reader);
    }
}



ListValue *read_list(Reader &reader) {
    reader.next(); // consume '('
    auto *list = new ListValue {};

    while (auto token = reader.peek() ) {
        if (*token == ")") {
            reader.next();
            break;
        }
        list->push(read_form(reader));
    }

    return list;
}


Value *read_atom(Reader &reader) {
    return new SymbolValue { *reader.next() };
}