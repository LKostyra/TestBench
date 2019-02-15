#pragma once

#include <cstdint>
#include <string>

struct BSTNode
{
    uint32_t key;
    std::string value;
    BSTNode* left;
    BSTNode* right;

    BSTNode(uint32_t k, const std::string& v)
        : key(k)
        , value(v)
        , left(nullptr)
        , right(nullptr)
    {
    }
};

class BST
{
    BSTNode* mRoot;

    void AddInternal(BSTNode* node, uint32_t k, const std::string& v);
    void PrintInternal(BSTNode* node, uint32_t depth);
    std::string FindInternal(BSTNode* node, uint32_t k);

public:
    BST();

    void Add(uint32_t k, const std::string& v);
    void Print();
    std::string Find(uint32_t k);
};
