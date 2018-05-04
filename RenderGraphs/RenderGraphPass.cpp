#include "RenderGraphPass.hpp"


RenderGraphPass::RenderGraphPass(uint32_t width, uint32_t height)
    : mWidth(width)
    , mHeight(height)
{
}

RenderGraphPass::~RenderGraphPass()
{
}

void RenderGraphPass::AddInputBuffer(RenderGraphNode::Ptr& ptr)
{
    AddInput(ptr);
}

void RenderGraphPass::AddInputTexture(RenderGraphNode::Ptr& ptr)
{
    AddInput(ptr);
}

void RenderGraphPass::AddOutputBuffer(RenderGraphNode::Ptr& ptr)
{
    AddOutput(ptr);
}

void RenderGraphPass::AddOutputTexture(RenderGraphNode::Ptr& ptr)
{
    AddOutput(ptr);
}
