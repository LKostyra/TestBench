#pragma once

#include "Common.hpp"
#include <memory>
#include <list>


class RenderGraphNode
{
public:
    using Ptr = std::shared_ptr<RenderGraphNode>;

    enum class Type: unsigned char
    {
        Unknown = 0,
        Pass,
        Resource,
    };

protected:
    std::list<RenderGraphNode::Ptr> mInputs;
    std::list<RenderGraphNode::Ptr> mOutputs;

public:
    RenderGraphNode();
    ~RenderGraphNode();

    TB_INLINE virtual Type GetType() const = 0;
};
