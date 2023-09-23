#include <unordered_map>
#include <cassert>

#include "../include/basic.h"
#include "../include/linenoise.hpp"
#include "../include/env.h"
#include "../include/reader.h"
#include "../include/printer.h"
#include "../include/types.h"


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
        return eval_ast(ast, env);
    } else if (ast->as_list()->is_empty()) {
        return ast;
    } else {
        
        auto list = ast->as_list();
        auto first = list->at(0);
        
        if (first->is_symbol()) {
            auto special = first->as_symbol();
            if (special->matches("def!")) {
                /*
                symbol "def!": 
                    call 
                        the set method of the current environment 
                        (second parameter of EVAL called env) 
                    using 
                        the unevaluated first parameter 
                        (second list element) as the symbol key 
                        and 
                        the evaluated second parameter as the value.
                */
                auto key = list->at(1)->as_symbol();
                auto val = EVAL(list->at(2), env);
                env.set(key, val);
                return val;//? nil

            } else if (special->matches("let*")) {
                /*
                symbol "let*": 
                    create a new environment 
                    using the current environment as the outer value 
                    and then use the first parameter as a list of new bindings 
                    in the "let*" environment. 

                    (let* [x "foo"
                            y "bar"] 
                            ...)

                    Take the second element of the binding list, 
                    call EVAL using the new "let*" environment 
                    as the evaluation environment, 
                    then call set on the "let*" environment 
                    using the first binding list element as the key 
                    and the evaluated second element as the value. 

                    This is repeated for each odd/even pair in the binding list. 

                    Note in particular, the bindings earlier in the list 
                    can be referred to by later bindings. 
                    Finally, the second parameter (third element) 
                    of the original let* form is evaluated 
                    using the new "let*" environment 
                    and the result is returned as the result 
                    of the let* (the new let environment 
                    is discarded upon completion).
                */
                auto new_env = new Env( &env );
                auto bindings = list->at(1)->as_list();
                for (size_t i=0; i<bindings->size(); i +=2) {
                    auto key = bindings->at(i)->as_symbol();
                    assert(i+1 < bindings->size());
                    auto val = EVAL(bindings->at(i+1), *new_env);
                    new_env->set(key, val);
                }
                return EVAL(list->at(2), *new_env);//memory leak
            
            } else if (special->matches("do")) {
                Value* result = nullptr;
                assert(list->size() > 1);
                for (size_t i = 1; i < list->size(); ++i) {
                    result = EVAL(list->at(i), env);
                }
                return result;
            } else if (special->matches("if")) {
                auto condition = list->at(1);
                auto true_expr = list->at(2);
                auto false_expr = list->size() >= 4 ? list->at(3) : new NilValue {};
                if (EVAL(condition, env)->is_truthy())
                    return EVAL(true_expr, env);
                else
                    return EVAL(false_expr, env);
            } else if (special->matches("fn*")) {
                auto env_ptr = &env;//todo: capture the pointer itself instead of the reference
                auto binds = list->at(1)->as_list();//first parameter from the outer scope
                auto body = list->at(2);//second paraemter (third list element of ast from outer scope)
                // create a closure:
                auto closure = [env_ptr, binds, body](size_t argc, Value **args) {
                    auto exprs = new ListValue {};//the parameters to the closure
                    for (size_t i = 0; i < argc; ++i)
                        exprs->push(args[i]);
                    auto fn_env = new Env { env_ptr, binds, exprs };
                    //return new NilValue {};
                    return EVAL(body, *fn_env);
                };//end closure
                return new FnValue { closure };
            }
        } 
        /*
        otherwise: 
            call eval_ast on the list 
            and apply the first element to the rest as before.
        */
        auto eval_list = eval_ast(ast, env)->as_list();
        auto fn = eval_list->at(0)->as_fn()->to_fn();
        Value* args[eval_list->size() - 1];
        for (size_t i = 1; i<eval_list->size(); ++i) {
            args[i - 1] = eval_list->at(i);
        }
        return fn(eval_list->size() -1, args);
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
        
        return env.get(ast->as_symbol());
        
        // auto search = env.find(ast->as_symbol());
        // if (search == env.end()) {
        //     throw new ExceptionValue { ast->as_symbol()->str() + " not found" };
        
        // return new SymbolValue {"nil"};
        // } 
        // return search->second;
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
    return new IntegerValue { result };// even integers are on the heap
    //TODO: (optional) use smart pointers to not heap allocate everything
}






int main() {
    //debugprint("Luke's CppLispy");
    const auto history_path = "history.txt";
    linenoise::LoadHistory(history_path);

    //Env env {};
    auto env = new Env { nullptr }; // top-level Env
    env->set(new SymbolValue("+") , new FnValue { add });//wrap funciton pointer in function value to make it like our lisp data type
    env->set(new SymbolValue("-") , new FnValue { sub });
    env->set(new SymbolValue("*") , new FnValue { mul });
    //env->set(new SymbolValue("/") , new FnValue { div });

    

    std::string input;
    for (;;) {
        auto quit = linenoise::Readline("user> ", input);
        if (quit) break;
        std::cout << rep(input, *env) << std::endl;
        // Add text to history
        linenoise::AddHistory(input.c_str());
    }
    linenoise::SaveHistory(history_path);

    return 0;
}