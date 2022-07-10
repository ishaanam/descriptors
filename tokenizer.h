#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <sstream>

#include "types.h"

struct Token
{
    std::string raw_token = "";
    TokenType type = TokenType::UNKNOWN;

    void reset()
    {
        raw_token = "";
        type = TokenType::UNKNOWN;
    }

    bool is_empty() const
    {
        if (raw_token == "" && type == TokenType::UNKNOWN) return true;
        return false;
    }
};

std::vector<Token> Tokenizer(std::stringstream& ss);

#endif
