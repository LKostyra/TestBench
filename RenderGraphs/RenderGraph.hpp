#pragma once

#include "RenderGraphNode.hpp"
#include "RenderGraphPass.hpp"
#include "RenderGraphResource.hpp"


class RenderGraph
{

public:
    RenderGraph();
    ~RenderGraph();

    void AddPass(RenderGraphPass::Ptr& pass);
    void AddInputBufferToPass(RenderGraphPass::Ptr& pass, RenderGraphBuffer::Ptr& buf);
    void AddInputTextureToPass(RenderGraphPass::Ptr& pass, RenderGraphTexture::Ptr& buf);
    void AddOutputBufferToPass(RenderGraphPass::Ptr& pass, RenderGraphBuffer::Ptr& buf);
    void AddOutputTextureToPass(RenderGraphPass::Ptr& pass, RenderGraphTexture::Ptr& buf);
};
