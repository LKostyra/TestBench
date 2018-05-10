#pragma once

#include "RenderGraphNode.hpp"
#include "RenderGraphPass.hpp"
#include "RenderGraphResource.hpp"


class RenderGraph
{
    // collects root passes, which begin the graph
    std::list<RenderGraphPass::Ptr> mRootPasses;

public:
    RenderGraph();
    ~RenderGraph();

    void AddRootPass(RenderGraphPass::Ptr& pass);
    bool VerifyGraph();

    static void AddInputBufferToPass(RenderGraphPass::Ptr& pass, RenderGraphBuffer::Ptr& buf);
    static void AddInputTextureToPass(RenderGraphPass::Ptr& pass, RenderGraphTexture::Ptr& tex);
    static void AddOutputBufferToPass(RenderGraphPass::Ptr& pass, RenderGraphBuffer::Ptr& buf);
    static void AddOutputTextureToPass(RenderGraphPass::Ptr& pass, RenderGraphTexture::Ptr& tex);
};
