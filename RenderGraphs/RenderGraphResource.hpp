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

protected:

public:
    RenderGraphResource();
    ~RenderGraphResource();

    TB_INLINE virtual Type GetResourceType() const = 0;

    TB_INLINE RenderGraphNode::Type GetType() const override
    {
        return RenderGraphNode::Type::Resource;
    }

    TB_INLINE void AddSourcePass(Ptr& ptr)
    {
        mInputs.push_back(ptr);
    }

    TB_INLINE void AddDestinationPass(Ptr& ptr)
    {
        mOutputs.push_back(ptr);
    }
};
