#pragma once

#include "Common.hpp"
#include <string>
#include <memory>
#include <list>


class RenderGraphNode
{
    friend class RenderGraph;

public:
    using Ptr = std::shared_ptr<RenderGraphNode>;

    enum class Type: unsigned char
    {
        Unknown = 0,
        Pass,
        Resource,
    };

protected:
    std::string mName;
    std::list<RenderGraphNode::Ptr> mInputs;
    std::list<RenderGraphNode::Ptr> mOutputs;

    TB_INLINE void AddInput(RenderGraphNode::Ptr ptr)
    {
        mInputs.push_back(ptr);
    }

    TB_INLINE void AddOutput(RenderGraphNode::Ptr ptr)
    {
        mOutputs.push_back(ptr);
    }

public:
    RenderGraphNode(const std::string& name);
    ~RenderGraphNode();

    TB_INLINE virtual Type GetType() const = 0;
};
