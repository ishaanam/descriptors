#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>

enum TokenType
{

    FUNCTION,
    END_FUNC,
    KEY,
    CHECKSUM,
    UNKNOWN
};

enum ScriptType
{
    SH,
    WSH,
    PK,
    PKH,
    WPKH,
    MULTI,
    SORTEDMULTI,
    ADDR,
    UNKNOWN_SCRIPT
};

enum KeyType
{
    COMPRESSED_PUBLIC_KEY,
    UNCOMPRESSED_PUBLIC_KEY,
    WIF_PK,
    XPUB,
    XPRV,
    NUM, // This is technically not a key type
    UNKNOWN_KEY
};

const std::unordered_map<std::string, ScriptType> script_types = {{"sh",ScriptType::SH}, {"wsh",ScriptType::WSH}, {"pk",ScriptType::PK}, {"pkh",ScriptType::PKH},
                                                                  {"wpkh",ScriptType::WPKH}, {"multi",ScriptType::MULTI}, {"sortedmulti",ScriptType::SORTEDMULTI},
                                                                  {"addr", ScriptType::ADDR}};

#endif
