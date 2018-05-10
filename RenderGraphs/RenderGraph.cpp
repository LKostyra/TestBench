#include "RenderGraph.hpp"
#include "Logger.hpp"


RenderGraph::RenderGraph()
{
}

RenderGraph::~RenderGraph()
{
}

void RenderGraph::AddRootPass(RenderGraphPass::Ptr& pass)
{
    mRootPasses.push_back(pass);
}

bool VerifyGraph()
{


    return true;
}

void RenderGraph::AddInputBufferToPass(RenderGraphPass::Ptr& pass, RenderGraphBuffer::Ptr& buf)
{
    pass->AddInput(std::dynamic_pointer_cast<RenderGraphNode>(buf));
    buf->AddDestinationPass(std::dynamic_pointer_cast<RenderGraphNode>(pass));
}

void RenderGraph::AddInputTextureToPass(RenderGraphPass::Ptr& pass, RenderGraphTexture::Ptr& tex)
{
    pass->AddInput(std::dynamic_pointer_cast<RenderGraphNode>(tex));
    tex->AddDestinationPass(std::dynamic_pointer_cast<RenderGraphNode>(pass));
}

void RenderGraph::AddOutputBufferToPass(RenderGraphPass::Ptr& pass, RenderGraphBuffer::Ptr& buf)
{
    pass->AddOutput(std::dynamic_pointer_cast<RenderGraphNode>(buf));
    buf->AddSourcePass(std::dynamic_pointer_cast<RenderGraphNode>(pass));
}

void RenderGraph::AddOutputTextureToPass(RenderGraphPass::Ptr& pass, RenderGraphTexture::Ptr& tex)
{
    pass->AddOutput(std::dynamic_pointer_cast<RenderGraphNode>(tex));
    tex->AddSourcePass(std::dynamic_pointer_cast<RenderGraphNode>(pass));
}
