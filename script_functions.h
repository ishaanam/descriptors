#ifndef SCRIPT_FUNCTIONS_H
#define SCRIPT_FUNCTIONS_H

#include "types.h"
#include "descriptor.h"

std::string get_script_string(const ScriptExpression& script_expr);

std::string sh_get_script(const ScriptExpression& script_expr);
std::string wsh_get_script(const ScriptExpression& script_expr);
std::string pk_get_script(const ScriptExpression& script_expr);
std::string pkh_get_script(const ScriptExpression& script_expr);
std::string wpkh_get_script(const ScriptExpression& script_expr);
std::string multi_get_script(const ScriptExpression& script_expr);

const std::unordered_map<ScriptType, std::string(*)(const ScriptExpression&)> script_functions = 
    {{ScriptType::SH, sh_get_script}, {ScriptType::WSH, wsh_get_script},{ScriptType::PK, pk_get_script},
     {ScriptType::PKH, pkh_get_script}, {ScriptType::WPKH, wpkh_get_script}, {ScriptType::MULTI, multi_get_script}
    };

#endif
