#include "RenderGraph.hpp"


RenderGraph::RenderGraph()
{
}

RenderGraph::~RenderGraph()
{
}

void RenderGraph::AddPass(RenderGraphPass::Ptr& pass)
{
}

void RenderGraph::AddInputBufferToPass(RenderGraphPass::Ptr& pass, RenderGraphBuffer::Ptr& buf)
{
    pass->AddInputBuffer(std::dynamic_pointer_cast<RenderGraphNode>(buf));
    buf->AddDestinationPass(std::dynamic_pointer_cast<RenderGraphNode>(pass));
}

void RenderGraph::AddInputTextureToPass(RenderGraphPass::Ptr& pass, RenderGraphTexture::Ptr& tex)
{
    pass->AddInputTexture(std::dynamic_pointer_cast<RenderGraphNode>(tex));
    tex->AddDestinationPass(std::dynamic_pointer_cast<RenderGraphNode>(pass));
}

void RenderGraph::AddOutputBufferToPass(RenderGraphPass::Ptr& pass, RenderGraphBuffer::Ptr& buf)
{
    pass->AddOutputBuffer(std::dynamic_pointer_cast<RenderGraphNode>(pass));
    buf->AddSourcePass(std::dynamic_pointer_cast<RenderGraphNode>(pass));
}

void RenderGraph::AddOutputTextureToPass(RenderGraphPass::Ptr& pass, RenderGraphTexture::Ptr& tex)
{
    pass->AddOutputTexture(std::dynamic_pointer_cast<RenderGraphNode>(tex));
    tex->AddSourcePass(std::dynamic_pointer_cast<RenderGraphNode>(pass));
}
