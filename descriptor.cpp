#include <iostream>
#include <string>

#include "descriptor.h"
#include "script_functions.h"

/*
    A bunch of functions related to getting information from output descriptors.
*/

std::string get_desc_address_type_string(const Descriptor& descriptor)
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

std::string get_descriptor_tree(Descriptor& descriptor)
{
    std::string desc_tree{""};
    int function_count{0};
    ScriptExpression* ptr_script_expr = &descriptor.script_expr;

    while (ptr_script_expr)
    {
        desc_tree += '\n';
        for (int i = 0; i < function_count; i++) desc_tree += '\t';
        desc_tree += ptr_script_expr->raw_script_function;
        function_count++;
        for (ScriptArg& script_arg : ptr_script_expr->script_args)
        {
            const bool correct_size = ptr_script_expr->script_args.size() == 1;
            ptr_script_expr = std::get_if<ScriptExpression>(&script_arg);
            if (ptr_script_expr) {
                assert (correct_size);
            } else if (KeyExpression* ptr_key_expr = std::get_if<KeyExpression>(&script_arg)) {
                desc_tree += '\n';
                for (int i = 0; i <= function_count; i++) desc_tree += '\t';
                switch (ptr_key_expr->get_key_type())
                {
                    case KeyType::COMPRESSED_PUBLIC_KEY:
                    case KeyType::UNCOMPRESSED_PUBLIC_KEY: {
                        desc_tree += "public key: ";
                        break;
                    }
                    case KeyType::WIF_PK: {
                        desc_tree += "private key: ";
                        break;
                    }
                    case KeyType::XPUB: {
                        desc_tree += "extended public key: ";
                        break;
                    }
                    case KeyType::XPRV: {
                        desc_tree += "extended private key: ";
                        break;
                    }
                    case KeyType::NUM: {
                        desc_tree += "multisig threshold: ";
                        break;
                    }
                    default:
                        break;
                }
                desc_tree += ptr_key_expr->get_raw_key();
            }
        }
    }
    return desc_tree;
}

bool is_solvable(Descriptor& descriptor)
{
    ScriptExpression* ptr_script_expr = &descriptor.script_expr;
    bool solvable = true;

    while (ptr_script_expr)
    {
        int threshold = 1;
        int reached_threshold = 0;
        bool key_expr = false;
        for (ScriptArg& script_arg : ptr_script_expr->script_args)
        {
            if (std::holds_alternative<ScriptExpression>(script_arg))
            {
                assert (ptr_script_expr->script_args.size() == 1);
                ScriptExpression& script_expr = std::get<ScriptExpression>(script_arg);
                ptr_script_expr = &script_expr;
            }
            else if (std::holds_alternative<KeyExpression>(script_arg))
            {
                key_expr = true;
                KeyExpression& key_expr = std::get<KeyExpression>(script_arg);
                if (key_expr.get_key_type() == KeyType::NUM) threshold = std::stoi(key_expr.get_raw_key());
                if (key_expr.is_private()) reached_threshold++;
                ptr_script_expr = nullptr;
            }
        }
        if (key_expr)
        {
            bool internal_solvable = reached_threshold >= threshold;
            solvable &= internal_solvable;
        }
    }
    return solvable;
}

void print_descriptor_info(Descriptor& descriptor)
{
    // eventually print:
    // checksum (if there is no checksum included, compute one)

    std::cout << get_desc_address_type_string(descriptor);
    std::cout << "solvable: " << (is_solvable(descriptor) ? "true" : "false") << '\n';
    std::cout << get_descriptor_tree(descriptor) << '\n';
    std::cout << '\n' << get_script_string(descriptor.script_expr) << '\n';
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
