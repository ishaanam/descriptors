#include <iostream>
#include <string>

#include "descriptor.h"

/*
    A bunch of functions related to getting information from output descriptors.
*/

std::string get_desc_address_type(const Descriptor& descriptor)
{
    const ScriptType& top_level_type = descriptor.script_expr.script_function;
    switch (top_level_type)
    {
        case ScriptType::SH :
            return "prefix: 3\naddress type: base58\n";
        case ScriptType::WSH :
            return "prefix: bc1q\naddress type: bech32\n";
        case ScriptType::PK :
            return "p2pk: no address type\n";
        case ScriptType::PKH :
            return "prefix: 1\naddress type: base58\n";
        case ScriptType::WPKH :
            return "prefix: bc1q\n address type: bech32\n";
        case ScriptType::MULTI :
            return "bare multisig: no address type\n";
        case ScriptType::SORTEDMULTI :
            return "bare multisig: no address type\n";
        case ScriptType::ADDR :
            return "script_expression already contains address prefix\n";
        default:
            return "unknown address type\n";
    }
}

void print_descriptor_info(const Descriptor& descriptor)
{
    // eventuallly print:
    // checksum (if there is no checksum included, compute one)
    // script type (so the standard ones like p2wsh, p2sh-p2wpkh, etc.)
    // are these scripts spendable using just this descriptor?
    // what an example script will look like
    std::cout << get_desc_address_type(descriptor);
}

void KeyExpression::parse_raw_key_expr(const std::string raw_key_expr)
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
