#include "BST.hpp"
#include <lkCommon/Utils/Logger.hpp>

const std::string SPACING("  ");

BST::BST()
    : mRoot(nullptr)
{
}

void BST::AddInternal(BSTNode* node, uint32_t k, const std::string& v)
{
    if (k > node->key)
    {
        if (!node->right)
        {
            node->right = new BSTNode(k, v);
        }
        else
        {
            AddInternal(node->right, k, v);
        }
    }
    else
    {
        if (!node->left)
        {
            node->left = new BSTNode(k, v);
        }
        else
        {
            AddInternal(node->left, k, v);
        }
    }
}

void BST::PrintInternal(BSTNode* node, uint32_t depth)
{
    std::string spacing;
    for (uint32_t i = 0; i < depth; ++i)
        spacing += SPACING;

    LOGI(spacing << "- " << node->key << ": " << node->value);

    if (node->left)
        PrintInternal(node->left, depth + 1);
    if (node->right)
        PrintInternal(node->right, depth + 1);
}

std::string BST::FindInternal(BSTNode* node, uint32_t k)
{
    if (node->key == k)
        return node->value;

    if (k > node->key)
    {
        // explore right branch
        if (node->right)
            return FindInternal(node->right, k);
        else
            return std::string("NOT FOUND");
    }
    else
    {
        // explore left branch
        if (node->left)
            return FindInternal(node->left, k);
        else
            return std::string("NOT FOUND");
    }
}

void BST::Add(uint32_t k, const std::string& v)
{
    if (!mRoot)
    {
        mRoot = new BSTNode(k, v);
    }
    else
    {
        AddInternal(mRoot, k, v);
    }
}

void BST::Print()
{
    LOGI("Tree dump:");
    PrintInternal(mRoot, 0);
}

std::string BST::Find(uint32_t k)
{
    if (!mRoot)
        return "";

    return FindInternal(mRoot, k);
}
