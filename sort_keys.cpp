#include "types.h"

constexpr int UNCOMPRESSED_PUBKEY_LEN = 130;
constexpr int COMPRESSED_PUBKEY_LEN = 66;

bool is_base_ten_int(const std::string& num)
{   
    for (const char& c : num)
    {
        if (!std::isdigit(c)) return false;
    }
        return true;
}

KeyType find_key_type(const std::string& key)
{
    const int key_len = key.length();

    switch(key_len)
    {
        case UNCOMPRESSED_PUBKEY_LEN: {
            if (key[1] == '4') return KeyType::UNCOMPRESSED_PUBLIC_KEY;
        }
        case COMPRESSED_PUBKEY_LEN: {
            if (key[1] == '2' || key[1] == '3') return KeyType::COMPRESSED_PUBLIC_KEY;
        }
        default: {
            if (key.substr(0,4) == "xpub") {
                return KeyType::XPUB;
            } else if (key.substr(0,4) == "xprv") {
                return KeyType::XPRV;
            } else if (key[0] == 'L' || key[0] == '5' || key[0] == 'K') {
                return KeyType::WIF_PK;
            } else if (is_base_ten_int(key)) {
                return KeyType::NUM;
            } else {
                return KeyType::UNKNOWN_KEY; 
            }
        }
    }
}
