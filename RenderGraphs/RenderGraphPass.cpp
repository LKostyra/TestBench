#include "RenderGraphPass.hpp"


RenderGraphPass::RenderGraphPass(const std::string& name, uint32_t width, uint32_t height)
    : RenderGraphNode(name)
    , mWidth(width)
    , mHeight(height)
{
}

RenderGraphPass::~RenderGraphPass()
{
}
