#include <iostream>
#include "descriptor.h"

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
