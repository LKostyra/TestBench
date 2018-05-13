#include "RenderGraphTexture.hpp"


RenderGraphTexture::RenderGraphTexture(const std::string& name, uint32_t width, uint32_t height)
    : RenderGraphResource(name)
    , mWidth(width)
    , mHeight(height)
{
}

RenderGraphTexture::~RenderGraphTexture()
{
}
