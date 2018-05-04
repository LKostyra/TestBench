#pragma once

#include "RenderGraphResource.hpp"
#include "RenderGraphBuffer.hpp"
#include "RenderGraphTexture.hpp"

#include <memory>
#include <list>


class RenderGraphPass: public RenderGraphNode
{
public:
    using Ptr = std::shared_ptr<RenderGraphPass>;

private:
    uint32_t mWidth;
    uint32_t mHeight;

public:
    RenderGraphPass(uint32_t width, uint32_t height);
    ~RenderGraphPass();

    // inputs/outputs for this pass
    void AddInputBuffer(RenderGraphNode::Ptr& ptr);
    void AddInputTexture(RenderGraphNode::Ptr& ptr);

    void AddOutputBuffer(RenderGraphNode::Ptr& ptr);
    void AddOutputTexture(RenderGraphNode::Ptr& ptr);

    TB_INLINE RenderGraphNode::Type GetType() const override
    {
        return RenderGraphNode::Type::Pass;
    }
};
