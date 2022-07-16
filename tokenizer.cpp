#include "tokenizer.h"

/*
    Converts a string stream to a vector of tokens, each with a
    given TokenType.
*/

std::vector<Token> Tokenizer(std::stringstream& ss)
{
    char c;
    Token current_token;

    std::vector<Token> tokens;

    while (ss>>c)
    {
        switch (c)
        {
            case '#':{
                current_token.type = TokenType::CHECKSUM;
                break;
            }
            case '(':{
                if (current_token.type == TokenType::KEY) current_token.type = TokenType::FUNCTION;
                tokens.push_back(current_token);
                current_token.reset();
                break;
            }
            case ')':{
                if (!current_token.is_empty()) tokens.push_back(current_token);
                current_token.raw_token = ")";
                current_token.type = TokenType::END_FUNC;
                tokens.push_back(current_token);
                current_token.reset();
                break;
            }
            case ',':{
                if (!current_token.is_empty())
                {
                    tokens.push_back(current_token);
                    current_token.reset();
                }
                break;
            }
            case '_':
            case '[':
            case ']':
            case '/': {
                current_token.raw_token += c;
                if (current_token.type == TokenType::UNKNOWN) current_token.type = TokenType::KEY;
                break;
            }
            default: {
                if (std::isalnum(c))
                {
                    current_token.raw_token += c;
                    if (current_token.type == TokenType::UNKNOWN) current_token.type = TokenType::KEY;
                }
            }
        }
    }
    // add last token to vector
    if (!current_token.is_empty()) tokens.push_back(current_token);

    return tokens;
}
