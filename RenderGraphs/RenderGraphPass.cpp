#include "RenderGraphPass.hpp"


RenderGraphPass::RenderGraphPass(uint32_t width, uint32_t height)
    : mWidth(width)
    , mHeight(height)
{
}

RenderGraphPass::~RenderGraphPass()
{
}

void RenderGraphPass::AddInputBuffer(RenderGraphBuffer::Ptr& ptr)
{
}

void RenderGraphPass::AddInputTexture(RenderGraphTexture::Ptr& ptr)
{
}

void RenderGraphPass::AddOutputBuffer(RenderGraphBuffer::Ptr& ptr)
{
}

void RenderGraphPass::AddOutputTexture(RenderGraphTexture::Ptr& ptr)
{
}
