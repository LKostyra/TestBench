#include "Hashing.hpp"

uint32_t Hash(const std::string& text)
{
    uint32_t hash = 5381;

    for (size_t i = 0; i < text.size(); ++i)
        hash = ((hash << 5) + hash) + (unsigned char)text[i];

    return hash;
}
