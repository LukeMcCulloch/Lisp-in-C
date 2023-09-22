#ifndef env_h
#define env_h


#include <unordered_map>

#include "../include/types.h"


class Env {

public:

    Env(Env* outer)
        : m_outer(outer) { }

    void set(SymbolValue* key, Value* val) {
        m_data[key] = val;
    }

    Env* find(const SymbolValue* key) const {
        auto search = m_data.find(key);
        if (search != m_data.end()) {
            return const_cast<Env*>(this);
        } else if (m_outer) {
            return m_outer->find(key);
        } else {
            return nullptr;
        }
    }

    Value* get(const SymbolValue* key) const {
        auto env = find(key);
        if (!env)
            throw new ExceptionValue { key->str() + " not found" };
        return env->m_data[key];
    }
    

private:

    Env *m_outer { nullptr };
    std::unordered_map<const SymbolValue *, Value *, HashMapHash, HashMapPredicate> m_data;
};



#endif /* __env_h */