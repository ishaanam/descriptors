#include "script_functions.h"

/*
    Functions that help display what a descriptor's corresponding script will look like.
    Note: currently some of these scripts will be invalid since if a private key is
    provided, then the script is constructed using that as opposed to the private key's
    corresponding private key.
*/

std::string get_script_string(const ScriptExpression& script_expr) {
    auto it = script_functions.find(script_expr.script_function);
    assert(it != script_functions.end());
    auto script_f = it->second;
    return script_f(script_expr);
}

std::string sh_get_script(const ScriptExpression& script_expr)
{
    std::string out{""};
    out += "OP_HASH160 hash160(" + get_script_string(std::get<ScriptExpression>(script_expr.script_args[0])) + ") OP_EQUAL";
    return out;
}

std::string wsh_get_script(const ScriptExpression& script_expr)
{
    std::string out{""};
    out += "OP_0 sha256(" + get_script_string(std::get<ScriptExpression>(script_expr.script_args[0])) + ")";
    return out;
}

std::string pk_get_script(const ScriptExpression& script_expr)
{
    std::string out{""};
    out += std::get<KeyExpression>(script_expr.script_args[0]).get_raw_key() + " OP_CHECKSIG";
    return out;

}

std::string pkh_get_script(const ScriptExpression& script_expr)
{
    std::string out{""};
    out += "OP_DUP OP_HASH160 hash160(" + std::get<KeyExpression>(script_expr.script_args[0]).get_raw_key() + ") OP_EQUALVERIFY OP_CHECKSIG";
    return out;

}

std::string wpkh_get_script(const ScriptExpression& script_expr)
{
    std::string out{""};
    out += "OP_0 hash160(" + std::get<KeyExpression>(script_expr.script_args[0]).get_raw_key() + ")";
    return out;

}

std::string multi_get_script(const ScriptExpression& script_expr)
{
    std::string out{""};
    if (script_expr.script_args.size() - 1 <= 16) out += "OP_"; 
    for (const ScriptArg& script_arg : script_expr.script_args)
    {
        const KeyExpression* ptr_key_expr = std::get_if<KeyExpression>(&script_arg);
        assert (ptr_key_expr);
        out += ptr_key_expr->get_raw_key();
    }
    out += "OP_CHECKMULTISIG";
    return out;
}

