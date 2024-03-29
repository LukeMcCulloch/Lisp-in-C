
#include <cassert>

#include "../include/reader.h"
#include "../include/basic.h"


std::vector<std::string_view> tokenize(std::string &input) {
    debugprint("tokenize");
    //debugprint("Hello1");
    Tokenizer tokenizer { input };
    std::vector<std::string_view> vector;
    while (auto token = tokenizer.next()) {
        vector.push_back(*token);
    }
    return vector;
}



//tokenize, parse into AST, return the AST
Value *read_str(std::string &input) {
    debugprint("read_str");
    auto tokens = tokenize(input);
    Reader reader { tokens };
    return read_form(reader);
}


Value *read_form(Reader &reader) {
    debugprint("read_form");
    auto maybe_token = reader.peek();

    if (!maybe_token) return nullptr;//garbage collection to be done

    auto token = maybe_token.value();
    
    switch (token[0]) {
        case '(':
            return read_list(reader);
        case '[':
            return read_vector(reader);
        case '{':
            return read_hash_map(reader);
        case '\'':
        case '`':
        case '~':
        case '@':
            return read_quoted_value(reader);
        case '^':
            return read_with_meta(reader);
        case '-':
            if (reader.peek().value().length() == 1) 
                return read_atom(reader);
            return read_integer(reader);
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return read_integer(reader);
        default:
        //assert(token.size() >= 1);
        if (token == "true") {
            reader.next();
            return TrueValue::the();
        } else if (token == "false") {
            reader.next();
            //return new FalseValue ();
            return FalseValue::the();
        } else if (token == "nil") {
            reader.next();
            //return new NilValue ();
            return NilValue::the();
        }
        return read_atom(reader);
    }
}


Value* read_integer(Reader &reader) { //maybe
    debugspot("read_integer", false);
    auto token = reader.next(); //do actually consume the token
    long num = 0;
    bool negative = false;
    for (char c : *token) {//optional token so dereference it
        if (c == '-') {
            negative = true;
        } else {
            num *= 10;//base 10
            int digit = c - 48; // 0
            num += digit;
        }
    }
    if (negative) num *= -1;

    return new IntegerValue { num };
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
    case '`': {
        reader.next();
        auto list = new ListValue {};
        list->push(new SymbolValue { "quasiquote" } );
        list->push(read_form(reader));
        return list;
    }
    case '~': {
        reader.next();
        auto list = new ListValue {};
        list->push(new SymbolValue { "unquote" } );
        list->push(read_form(reader));
        return list;
    }
    case '@': {
        if (token.value().length() > 1 && token.value()[1] == '@') {
            reader.next();
            auto list = new ListValue {};
            list->push(new SymbolValue { "splice-unquote" } );
            list->push(read_form(reader));
            return list;
        } else {
            reader.next();
            auto list = new ListValue {};
            list->push(new SymbolValue { "deref" } );
            list->push(read_form(reader));
            return list;
        }
    }
    default:
        std::cerr << "bad quote\n";
        abort();
    }

}


Value* read_with_meta(Reader &reader) {
    reader.next(); // consume '^'

    auto list = new ListValue {};
    auto meta = read_form(reader);
    auto value = read_form(reader);
    list->push(new SymbolValue { "with-meta" } );
    list->push(value);
    list->push(meta);
    return list;
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