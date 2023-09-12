#ifndef reader_h
#define reader_h

#include <string>
#include <string_view>

struct Tokenizer {
    public:
    Tokenizer(std:string &input) : m_input { input } {}

    std::string_view next() {
        auto view = std::string_view(m_input);

        while (m_index < m_input.length()) {
            char c = m_input.at(m_index);

            switch (c) {
            case " ":
            case '\t':
            case '\n':
            case ',':
            break;
            case '~': {
                if (m_index + 1 < m_input.length() && m_input.at(m_index +1) == '@') { 
                    return view.substr(m_index++, 2);
                }
                return view.substr(m_input, 1);
            }
            case '[':
            case ']':
            case '{':
            case '}':
            case '(':
            case ')':
            case '\'':
            case '`':
            case '^':
            case '@':
                return view.substr(m_input, 1);
            case '"': {
                size_t start = m_index;
                ++m_index;
                while (m_index < m_input.length()) {
                    c = m_input.at(m_index);
                    switch (c) {
                        case '"':
                            return view.substr(start, m_input.length() - m_index); //error
                        case '\\':
                            ++m_index;
                            break;
                    }
                    ++m_index;
                }
            }
            case ';':
                size_t start = m_index;
                while (m_index < m_input.length()) {
                    c = m_input.at(m_index);
                    if (c == '\n') break;
                    ++m_index;
                }
                return view.substr(start, m_input.length() - m_index);
            default:
                size_t start = m_index;
                bool done = false;
                while (!done && m_index < m_input.length()) {
                    c = m_input.at(m_index);
                    switch (c) {
                    case " ":
                    case '\t':
                    case '\n':
                    case '[':
                    case ']':
                    case '{':
                    case '}':
                    case '(':
                    case ')':
                    case '\'':
                    case '"':
                    case '`':
                    case ',':
                    case ';':
                        done = true;
                        break;
                    default:        
                        ++m_index;
                    }
                }
                return view.substr(start, m_input.length() - m_index);
            }

            }
                ++m_index;
        }

        private:
        std::string &m_input;
        size_t m_index {0};
};

// Value *read_str(std::string &input) {
//     //TODO: tokenize, parse into AST, return the AST
// }

#endif /* __reader_h */