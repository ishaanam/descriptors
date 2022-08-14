#include <stdexcept>
// REMOVE
#include <iostream>

#include "tokenizer.h"
#include "descriptor.h"

/*
    Converts a vector of tokens generated by Tokenizer() into an object
    of the Descriptor class.
*/

ScriptType string_to_script_type(const std::string& script_string)
{
    auto it = script_types.find(script_string);
    if (it == script_types.end()) throw std::runtime_error("unrecognized function '" + script_string + "'");
    return it->second;
}

void parse_function(const std::vector<Token>& tokens, ScriptExpression* script_expr, int function_index)
{
    const Token& current_token = tokens[function_index];
    script_expr->raw_script_function = current_token.raw_token;
    script_expr->script_function =  string_to_script_type(current_token.raw_token);
    Token next_token = tokens[function_index + 1];

    switch (script_expr->script_function)
    {
        case ScriptType::MULTI: {
            // while the token isn't an END_FUNC, everything else from here is a key (not a NUM)
            if (next_token.type != TokenType::KEY) throw std::runtime_error("The arguments for a " + current_token.raw_token + "() function must be key expressions");
            KeyExpression multi_threshold;
            multi_threshold.parse_raw_key_expr(next_token.raw_token);
            multi_threshold.set_key_type();
            function_index += 2;
            next_token = tokens[function_index++];
            if (multi_threshold.get_key_type() != KeyType::NUM) throw std::runtime_error("The first argument for a " + current_token.raw_token +"() function must be an unsigned integer");
            script_expr->script_args.push_back(multi_threshold);
            while(next_token.type != TokenType::END_FUNC)
            {
                KeyExpression key_expr;
                key_expr.parse_raw_key_expr(next_token.raw_token);
                key_expr.set_key_type();
                if (key_expr.get_key_type() == KeyType::UNKNOWN_KEY) throw std::runtime_error("Key '" + key_expr.get_raw_key() + "' has unknown type");

                script_expr->script_args.push_back(key_expr);
                next_token = tokens[function_index++];
            }
            break;
        }
        case ScriptType::SH:
        case ScriptType::WSH: {
            // the next token must be a script expression
            if (next_token.type != TokenType::FUNCTION) throw std::runtime_error("The argument for a " + current_token.raw_token + "() function must be a script expression");
            // set the script now
            ScriptExpression inner_script_expr;
            script_expr->script_args.push_back(inner_script_expr);
            break;
        }
        case ScriptType::PK:
        case ScriptType::PKH:
        case ScriptType::WPKH: {
            // the next token must be a key expression
            if (next_token.type != TokenType::KEY) throw std::runtime_error("The argument for a " + current_token.raw_token + "() function must be a key expression");
            // set the key now
            KeyExpression key_expr;
            key_expr.parse_raw_key_expr(next_token.raw_token);
            key_expr.set_key_type();
            if (key_expr.get_key_type() == KeyType::UNKNOWN_KEY) throw std::runtime_error("Key '" + key_expr.get_raw_key() + "' has unknown type");
            script_expr->script_args.push_back(static_cast<ScriptArg>(key_expr));
            // ensure that the next token is an END_FUNC
            if (tokens[function_index + 2].type != TokenType::END_FUNC) {
                if (tokens[function_index + 2].type == TokenType::KEY || tokens[function_index + 2].type == TokenType::FUNCTION) {
                    throw std::runtime_error("The script " + current_token.raw_token + "() function can only have a single key expression");
                } else {
                    throw std::runtime_error("Missing a parentheses after '" + current_token.raw_token + "(" + next_token.raw_token + "'");
                }
            }
            break;
        }
        case ScriptType::SORTEDMULTI:
            throw std::runtime_error("This parser doesn't currently support '" + tokens[function_index].raw_token + "' type script expressions");
        default:
            break;
    }
}

Descriptor tokens_to_descriptor(const std::vector<Token>& tokens)
{
    Descriptor descriptor;
    ScriptExpression* script_expr = &descriptor.script_expr;

    for (int i=0; i < tokens.size(); i++)
    {
        const Token& token = tokens[i];
        switch (token.type)
        {
            case (TokenType::FUNCTION): {
                parse_function(tokens, script_expr, i);
                if (std::holds_alternative<ScriptExpression>(script_expr->script_args[0]))
                {
                    script_expr = &std::get<ScriptExpression>(script_expr->script_args[0]);
                }
                break;
            }
            case (TokenType::CHECKSUM): {
                descriptor.checksum = token.raw_token;
                // this must be the last token
                if (tokens[i+1].raw_token != "") throw std::runtime_error("The checksum '" + token.raw_token + "' can't be followed by another expression");
                break;
            }
            case (TokenType::KEY):
                break;
            case (TokenType::END_FUNC): {
                const Token& next_token = tokens[i+1];
                if (next_token.type != TokenType::END_FUNC && next_token.raw_token != "") throw std::runtime_error("A script expression can't be followed by '" + next_token.raw_token + "'");
            }
            default:
                break;
        }
    }
    return descriptor;
}

Descriptor ss_to_descriptor(std::stringstream ss)
{
    const std::vector<Token> tokens = Tokenizer(ss);
    return tokens_to_descriptor(tokens);
}
