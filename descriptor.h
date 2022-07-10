#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <string>
#include <vector>
#include <optional>
#include <sstream>

#include "sort_keys.h"
#include "types.h"

/*
    This file contains the majority of the descriptor logic which includes the
    descriptor class.
*/

struct ScriptExpression;

typedef std::vector<std::string> DerivationPath; 

struct KeyExpression
{
private:
    KeyType key_type;
    std::string raw_key;
    DerivationPath derivation_path;
public:
    void set_key_type()
    {
        assert (raw_key != "");
        key_type = find_key_type(raw_key);
    }
    void parse_raw_key_expr(const std::string raw_key_expr)
    {
        if (raw_key_expr[0] != '[') 
        {
            raw_key = raw_key_expr; 
        } else {
            std::stringstream ss {raw_key_expr};
            char c;
            std::string tmp_index;
            bool is_deriv = true;
            while (ss>>c)
            {
                switch (c)
                {
                    case '[':
                        break;
                    case ']': {
                        derivation_path.push_back(tmp_index);
                        tmp_index = "";
                        is_deriv = false;
                        break;
                    }
                    case '/': {
                        if (is_deriv) {
                            derivation_path.push_back(tmp_index);
                            tmp_index = "";
                        }
                        break;
                    } default: {
                        tmp_index += c;
                        break;
                    }
                }
            }
            raw_key = tmp_index;
        }
    }

    KeyType get_key_type() const {return key_type;}
    std::string get_raw_key() const {return raw_key;}
    DerivationPath get_derivation_path() const {return derivation_path;}
};

typedef std::variant<ScriptExpression, KeyExpression>  ScriptArg;

struct ScriptExpression
{
    ScriptType script_function;
    std::vector<ScriptArg> script_args;
};

struct Descriptor
{
    std::optional<std::string> checksum;
    ScriptExpression script_expr;
};

#endif
