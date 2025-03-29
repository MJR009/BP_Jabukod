#include "Escapes.h"

string Escapes::ReplaceEscapeSequences(const string & str) {
    string resolved = "";
    
    for (size_t i = 1; i < str.size() - 1; i++) { // literal itself is delimited by "", they are skipped
        if (str[i] == '\\') {
            i++;
            switch (str[i]) {
                case '\"':
                    resolved.push_back('\"');
                    break;
                case '\\':
                    resolved.push_back('\\');
                    break;
                case 'n':
                    resolved.push_back('\n');
                    break;
                case 't':
                    resolved.push_back('\t');
                    break;
            }
        } else {
            resolved.push_back(str[i]);
        }
    }

    return resolved;
}
