#include "RenderGraphBuffer.hpp"


RenderGraphBuffer::RenderGraphBuffer(const std::string& name, uint32_t size)
    : RenderGraphResource(name)
    , mSize(size)
{
}

RenderGraphBuffer::~RenderGraphBuffer()
{
}
