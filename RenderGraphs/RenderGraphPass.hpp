#pragma once

#include "RenderGraphResource.hpp"
#include "RenderGraphBuffer.hpp"
#include "RenderGraphTexture.hpp"

#include <memory>
#include <list>


class RenderGraphPass: public RenderGraphNode
{
    friend class RenderGraph;

public:
    using Ptr = std::shared_ptr<RenderGraphPass>;

private:
    uint32_t mWidth;
    uint32_t mHeight;

public:
    RenderGraphPass(uint32_t width, uint32_t height);
    ~RenderGraphPass();

    TB_INLINE RenderGraphNode::Type GetType() const override
    {
        return RenderGraphNode::Type::Pass;
    }
};
