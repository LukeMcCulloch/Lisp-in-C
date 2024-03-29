#include "../include/types.h"
#include "../include/basic.h"




ListValue *Value::as_list() {
    //debugprint("Hello1");
    assert(type() == Type::List || type() == Type::Vector);
    return static_cast<ListValue*>(this);
}

VectorValue *Value::as_vector() {
    assert(type() == Type::Vector);
    return static_cast<VectorValue*>(this);

}


HashMapValue* Value::as_hash_map() {
    assert(type() == Type::HashMap);
    return static_cast<HashMapValue*>(this);

}

SymbolValue* Value::as_symbol() {
    assert(type() == Type::Symbol);
    return static_cast<SymbolValue*>(this);

}

// IntegerValue* Value::as_integer() {
//     assert(type() == Type::Integer);
//     return static_cast<IntegerValue*>(this);

// }
IntegerValue* Value::as_integer() {
    assert(type() == Type::Integer);
    return static_cast<IntegerValue *>(this);
}

FnValue* Value::as_fn() {
    assert(type() == Type::Fn);
    return static_cast<FnValue*>(this);

}

ExceptionValue* Value::as_exception() {
    assert(type() == Type::Exception);
    return static_cast<ExceptionValue*>(this);

}


TrueValue *Value::as_true() {
    assert(type() == Type::True);
    return static_cast<TrueValue *>(this);
}

FalseValue *Value::as_false() {
    assert(type() == Type::False);
    return static_cast<FalseValue *>(this);
}

NilValue *Value::as_nil() {
    assert(type() == Type::Nil);
    return static_cast<NilValue *>(this);
}


const StringValue *Value::as_string() const {
    assert(type() == Type::String);
    return static_cast<const StringValue *>(this);
}



std::string ListValue::inspect(bool print_readably) const {
    std::string out = "(";
    for (auto *value : m_list) {
        out.append(value->inspect(print_readably));
        out.append(" ");
    }
    if (m_list.size() > 0) {
        out[out.length() - 1] = ')';
    } else {
        out.append(")");
    }
    return out;
}





std::string VectorValue::inspect(bool print_readably) const {
    std::string out = "[";
    for (auto *value : m_list) {
        out.append(value->inspect(print_readably));
        out.append(" ");
    }
    if (m_list.size() > 0) {
        out[out.length() - 1] = ']';
    } else {
        out.append("]");
    }
    return out;
}



std::string HashMapValue::inspect(bool print_readably) const {
    std::string out = "{";
    for (auto pair : m_map) {
        out.append(pair.first->inspect(print_readably));
        out.append(" ");
        out.append(pair.second->inspect(print_readably));
        out.append(" ");
    }
    if (m_map.size() > 0) {
        out[out.length() - 1] = '}';
    } else {
        out.append("}");
    }
    return out;
}


std::string StringValue::inspect(bool print_readably) const {
    if (print_readably) {
        std::string str = "\"";
        for (char c : m_str) {
            switch (c) {
            case '"':
                str += '\\';
                str += c;
                break;
            case '\\':
                str += '\\';
                str += '\\';
                break;
            case '\n':
                str += '\\';
                str += 'n';
                break;
            default:
                str += c;
            }
        }
        str += "\"";
        return str;
    }
    return m_str;
}