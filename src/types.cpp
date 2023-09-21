#include "../include/types.h"
#include "../include/basic.h"

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