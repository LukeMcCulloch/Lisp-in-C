#ifndef types_h
#define types_h

#include <iostream>
#include <cassert>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "../include/basic.h"

//fwd declare
class ListValue;
class VectorValue;
class HashMapValue;
class SymbolValue;
class IntegerValue;
class FnValue;
class ExceptionValue;

class Value {
public:

    enum class Type {
        List,
        Vector,
        HashMap,
        Symbol,
        Integer,
        Fn,
        Exception,
    };

    virtual Type type() const = 0;
    virtual std::string inspect() const = 0;

    virtual bool is_symbol() const { return false;}
    //}

    //cast it if you have a pointer to the base class:
    ListValue* as_list();
    VectorValue* as_vector();
    HashMapValue* as_hash_map();
    SymbolValue* as_symbol();
    IntegerValue* as_integer();
    FnValue* as_fn();
    ExceptionValue* as_exception();
};




class ListValue : public Value {

public:
    ListValue() { }

    void push(Value *value) {
        m_list.push_back(value);
    }

    virtual Type type() const override { return Type::List; }
    virtual std::string inspect() const override;

    auto begin() { return m_list.begin(); }
    auto end() { return m_list.end(); }
    

    bool is_empty() {return m_list.size() == 0;}
    size_t size() { return m_list.size();}
    Value* at(size_t index) { return m_list.at(index); }

protected:
    std::vector<Value*> m_list {};
};



class VectorValue : public ListValue {

public:
    VectorValue() {}
    virtual Type type() const override { return Type::Vector; }
    virtual std::string inspect() const override;
};



struct HashMapHash
{
    std::size_t operator()(Value* key) const noexcept {
        return std::hash<std::string> {}(key->inspect());
    }
    std::size_t operator()(const Value* key) const noexcept {
        return std::hash<std::string> {}(key->inspect());
    }
};

struct HashMapPredicate {
    bool operator()(const Value* lhs, const Value* rhs) const {
        return lhs->inspect() == rhs->inspect(); // Fixme pointer comparison... we need a real equality later
    }
};

class HashMapValue : public Value {

public:
    HashMapValue () { }

    virtual Type type() const override { return Type::HashMap; }
    virtual std::string inspect() const override;

    void set(Value* key, Value* value) {
        m_map[key] = value;
    }

    Value* get(Value* key) {
        auto search = m_map.find(key);
        if (search != m_map.end()) return search->second;
        return nullptr;
    }

    
    auto begin() { return m_map.begin(); }
    auto end() { return m_map.end(); }

private:
    std::unordered_map<Value*, Value*, HashMapHash, HashMapPredicate> m_map;

};


class SymbolValue : public Value {

public:
    SymbolValue(std::string_view str) 
        : m_str { str } {}
    
    std::string str() const { return m_str; }

    bool matches(const char *str) const { return m_str == str;}

    virtual Type type() const override { return Type::Symbol; }

    virtual std::string inspect() const override {
        debugprint("SymbolValue::inspect");
        return str();
    }

    virtual bool is_symbol() const override { return true;}
    
    //std::string SymbolValue:: inspect();
private:
    std::string m_str;
};


class IntegerValue : public Value {
public:
    IntegerValue( long l) : m_long { l } {}

    
    virtual Type type() const override { return Type::Integer; }
    virtual std::string inspect() const override {
        return std::to_string(m_long);
    }

    long to_long() { return m_long;}

private:
    long m_long { 0 };
};


// function pointer, 1.) old fashioned way
using FnPtr = Value *(*)(size_t, Value**);

// do this the old fashioned way for now (use variadic templates later)
class FnValue : public Value {
public:
    FnValue( FnPtr fn) : m_fn { fn } {}

    
    virtual Type type() const override { return Type::Fn; }
    virtual std::string inspect() const override {
        return "<fn>";
    }

    FnPtr to_fn() { return m_fn; }


private:
    FnPtr m_fn { nullptr };
};



// do this the old fashioned way for now (use variadic templates later)
class ExceptionValue : public Value {
public:
    ExceptionValue( std::string message)
        : m_message {message} {}


    
    virtual Type type() const override { return Type::Exception; }

    virtual std::string inspect() const override {
        return "<exception" + m_message + ">";
    }

    const std::string &message() { return m_message; }


private:
    std::string m_message;
};

#endif /* __types_h */