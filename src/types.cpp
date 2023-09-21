#include "../include/types.h"
#include "../include/basic.h"



ListValue* Value::as_list() {
    assert(type()) == Type::List;
    return static_cast<ListValue*>(this);
}

VectorValue* Value::as_vector() {
    assert(type()) == Type::Vector;
    return static_cast<VectorValue*>(this);

}

HashMapValue* Value::as_hash_map() {
    assert(type()) == Type::HashMap;
    return static_cast<FnValue*>(this);

}

SymbolValue* Value::as_symbol() {
    assert(type()) == Type::Symbol;
    return static_cast<HashMapValue*>(this);

}

IntegerValue* Value::as_integer() {
    assert(type()) == Type::Integer;
    return static_cast<IntegerValue*>(this);

}

FnValue* Value::as_fn() {
    assert(type()) == Type::Fn;
    return static_cast<FnValue*>(this);

}


std::string ListValue::inspect() {
    
    debugprint("ListValue::inspect");
    std::string out = "(";
    for (auto *value : m_list) {
        out.append(value->inspect());
        out.append(" ");
    };
    if (m_list.size() > 0) {
        out[out.length() - 1] = ')';
    } else {
        out.append(")");
    }
    //out[out.length() - 1] = ')';
    return out;
}



std::string VectorValue::inspect() {
    
    debugprint("VectorValue::inspect");
    std::string out = "[";
    for (auto *value : m_list) {
        out.append(value->inspect());
        out.append(" ");
    };
    if (m_list.size() > 0) {
        out[out.length() - 1] = ']';
    } else {
        out.append("]");
    }
    //out[out.length() - 1] = ')';
    return out;
}



std::string HashMapValue::inspect() {
    
    debugprint("HashMapValue::inspect");
    std::string out = "{";
    for (auto pair : m_map) {
        out.append(pair.first->inspect());
        out.append(" ");
        out.append(pair.second->inspect());
        out.append(" ");
    };
    if (m_map.size() > 0) {
        out[out.length() - 1] = '}';
    } else {
        out.append("}");
    }
    return out;
}