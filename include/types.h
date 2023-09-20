#ifndef types_h
#define types_h

#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>

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

private:
    std::vector<Value*> m_list {};
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