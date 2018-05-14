#pragma once

#include "RenderGraphNode.hpp"
#include <memory>
#include <list>


class RenderGraphResource: public RenderGraphNode
{
    friend class RenderGraph;

public:
    using Ptr = std::shared_ptr<RenderGraphResource>;

    enum class Type: unsigned char
    {
        Unknown = 0,
        Buffer,
        Texture,
    };

    RenderGraphResource(const std::string& name);
    ~RenderGraphResource();

    LKCOMMON_INLINE virtual Type GetResourceType() const = 0;

    LKCOMMON_INLINE RenderGraphNode::Type GetType() const override
    {
        return RenderGraphNode::Type::Resource;
    }

    LKCOMMON_INLINE void AddSourcePass(RenderGraphNode::Ptr ptr)
    {
        AddInput(ptr);
    }

    LKCOMMON_INLINE void AddDestinationPass(RenderGraphNode::Ptr ptr)
    {
        AddOutput(ptr);
    }
};
