#include <unordered_map>
#include <cassert>

#include "../include/basic.h"
#include "../include/linenoise.hpp"
#include "../include/reader.h"
#include "../include/printer.h"
#include "../include/types.h"


using Env = std::unordered_map<SymbolValue*, Value*, HashMapHash, HashMapPredicate>;


//fwd declare"
Value* eval_ast(Value *ast, Env &env);

Value* READ(std::string input) {
    debugprint("read");
    return read_str(input);
}

Value* EVAL(Value *ast, Env &env) {
    debugprint("EVAL");
    /*
    ast is not a list: 
        then return the result of calling eval_ast on it.
    ast is a empty list: 
        return ast unchanged.
    ast is a list: 
        call eval_ast to get a new evaluated list. 
        Take the first item of the evaluated list and call it as function 
        using the rest of the evaluated list as its arguments.
    */
    if (ast->type() != Value::Type::List) {
        debugprint("EVAL1");
        return eval_ast(ast, env);
    } else if (ast->as_list()->is_empty()) {
        debugprint("EVAL2");
        return ast;
    } else {
        debugprint("EVAL3");
        auto list = eval_ast(ast, env)->as_list();
        auto fn = list->at(0)->as_fn()->to_fn();
        Value* args[list->size() - 1];
        for (size_t i = 1; i<list->size(); ++i) {
            args[i - 1] = list->at(i);
        }
        return fn(list->size() -1, args);
    }
}




Value* eval_ast(Value *ast, Env &env) {
    debugprint("eval_ast");
    /*
    inputs: 
        ast (mal data type) 
        associative structure (the environment from above). 
        
        eval_ast switches on the type of ast as follows:
        
        symbol: 
            lookup the symbol in the environment structure and return the value 
            or raise an error if no value is found
        list: 
            return a new list that is the result of calling EVAL on each of the members of the list
        otherwise: 
            just return the original ast value
    */
    switch (ast->type()) {
    case Value::Type::Symbol: {
        //debugspot(ast->inspect(),true);
        auto search = env.find(ast->as_symbol());
        if (search == env.end()) {
            throw new ExceptionValue { ast->as_symbol()->str() + " not found" };
        
        return new SymbolValue {"nil"};
        } 
        return search->second;
    }
    case Value::Type::List: {
        auto result = new ListValue {};
        for (auto val : *ast->as_list()) { //ast must be dereferenced!
            result->push(EVAL(val, env));
        }
        return result;
    }
    case Value::Type::Vector: {
        auto result = new VectorValue {};
        for (auto val : *ast->as_vector()) { //ast must be dereferenced!
            result->push(EVAL(val, env));
        }
        return result;
    }
    case Value::Type::HashMap: {
        auto result = new HashMapValue {};
        for (auto pair : *ast->as_hash_map()) { //ast must be dereferenced!
            auto val = EVAL(pair.second, env);
            result->set(pair.first, val);
        }
        return result;
    }
    default:
        return ast;
    }
}



std::string PRINT(Value* input) {
    debugprint("PRINT");
    return pr_str(input);
}



std::string rep(std::string input, Env &env) {
    try {
        debugprint("rep");
        auto ast    = READ(input);
        if (ast) { //todo: fixme hack -- fix 2 for the null input error
            auto result = EVAL(ast, env);
            return PRINT(result);
        } else {
            return "";
        }
    } catch (ExceptionValue* exception) {
        std::cerr << exception->message() <<std::endl;
        return "";
    }
}


Value* add(size_t argc, Value** args) {
    assert(argc ==2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() + b->as_integer()->to_long();
    return new IntegerValue { result };// even integers are on the heap
    //TODO: (optional) use smart pointers to not heap allocate everything
}



Value* sub(size_t argc, Value** args) {
    assert(argc ==2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() - b->as_integer()->to_long();
    std::cout << "sub: " << result << std::endl;
    return new IntegerValue { result };// even integers are on the heap
    //TODO: (optional) use smart pointers to not heap allocate everything
}



Value* mul(size_t argc, Value** args) {
    assert(argc ==2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() * b->as_integer()->to_long();
    std::cout << "sub: " << result << std::endl;
    return new IntegerValue { result };// even integers are on the heap
    //TODO: (optional) use smart pointers to not heap allocate everything
}



Value* div(size_t argc, Value** args) {
    assert(argc ==2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() / b->as_integer()->to_long();
    std::cout << "sub: " << result << std::endl;
    //debugspot(77, true);
    return new IntegerValue { result };// even integers are on the heap
    //TODO: (optional) use smart pointers to not heap allocate everything
}






int main() {
    //debugprint("Luke's CppLispy");
    const auto history_path = "history.txt";
    linenoise::LoadHistory(history_path);

    Env env {};
    env[new SymbolValue("+")] = new FnValue { add };//wrap funciton pointer in function value to make it like our lisp data type
    env[new SymbolValue("-")] = new FnValue { sub };
    env[new SymbolValue("*")] = new FnValue { mul };
    env[new SymbolValue("/")] = new FnValue { div };

    std::string input;
    for (;;) {
        auto quit = linenoise::Readline("user> ", input);
        if (quit) break;
        std::cout << rep(input, env) << std::endl;
        // Add text to history
        linenoise::AddHistory(input.c_str());
    }
    linenoise::SaveHistory(history_path);

    return 0;
}