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
        case '[':
            return read_vector(reader);
        case '{':
            return read_hash_map(reader);
        case '\'':
            return read_quoted_value(reader);
        default:
            return read_atom(reader);
    }
}

Value* read_quoted_value(Reader &reader) {
    auto token = reader.peek();

    switch (token.value()[0]) {
    case '\'': {
        reader.next();
        auto list = new ListValue {};
        list->push(new SymbolValue { "quote" } );
        list->push(read_form(reader));
        return list;
    }
    default:
        std::cerr << "bad quote\n";
        abort();
    }

}

ListValue *read_list(Reader &reader) {
    reader.next(); // consume '('
    auto *list = new ListValue {};

    while (auto token = reader.peek() ) {
        if (*token == ")") {
            reader.next();
            return list;
        }
        list->push(read_form(reader));
    }

    std::cerr << "EOF\n";
    return list;
}


VectorValue *read_vector(Reader &reader) {
    reader.next(); // consume '['
    auto *vector = new VectorValue {};

    while (auto token = reader.peek() ) {
        if (*token == "]") {
            reader.next();
            return vector;
        }
        vector->push(read_form(reader));
    }

    std::cerr << "EOF\n";
    return vector;
}


HashMapValue *read_hash_map(Reader &reader) {
    reader.next(); // consume '{'
    auto *map = new HashMapValue {};

    while (auto token = reader.peek() ) {
        if (*token == "}") {
            reader.next();
            return map;
        }

        auto key = read_form(reader);
        token = reader.peek();
        if (*token == "}") {
            std::cerr << "hash-map key without value!\n";
            reader.next();
            return map;
        }
        
        auto value = read_form(reader);

        map->set(key, value);
    }

    std::cerr << "EOF\n";
    return map;
}


Value *read_atom(Reader &reader) {
    return new SymbolValue { *reader.next() };
}