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

