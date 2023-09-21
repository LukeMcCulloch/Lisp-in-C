#ifndef types_h
#define types_h

#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "../include/basic.h"

class Value {
public:
    virtual std::string inspect() { 
        debugprint("Value::inspect");
        assert(0); 
    }
};

//     virtual std::string inspect() { 
//         debugprint("Value::inspect");
//         return ""; } //{ assert(0); }
// };



class ListValue : public Value {

public:
    ListValue() {}

    void push(Value *value) {
        m_list.push_back(value);
    }

    virtual std::string inspect();

protected:
    std::vector<Value*> m_list {};
};



class VectorValue : public ListValue {

public:
    VectorValue() {}
    virtual std::string inspect();
};



struct HashMapHash
{
    std::size_t operator()(Value* key) const noexcept {
        return std::hash<std::string> {}(key->inspect());
    }
};

struct HashMapPredicate {
    constexpr bool operator()(Value* lhs, Value* rhs) const {
        return lhs == rhs; // Fixme pointer comparison... we need a real equality later
    }
};

class HashMapValue : public Value {

public:
    HashMapValue () { }

    virtual std::string inspect();

    void set(Value* key, Value* value) {
        m_map[key] = value;
    }

    Value* get(Value* key) {
        auto search = m_map.find(key);
        if (search != m_map.end()) return search->second;
        return nullptr;
    }

private:
    std::unordered_map<Value*, Value*, HashMapHash, HashMapPredicate> m_map;

};


class SymbolValue : public Value {

public:
    SymbolValue(std::string_view str) 
        : m_str { str } {}
    
    std::string str() { return m_str; }

    virtual std::string inspect() {
        debugprint("SymbolValue::inspect");
        return str();
    }
    
    //std::string SymbolValue:: inspect();
private:
    std::string m_str;
};


#endif /* __types_h */