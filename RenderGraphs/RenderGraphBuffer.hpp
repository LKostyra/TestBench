#pragma once

#include "RenderGraphResource.hpp"


class RenderGraphBuffer: public RenderGraphResource
{
public:
    using Ptr = std::shared_ptr<RenderGraphBuffer>;

private:
    uint32_t mSize;

public:
    RenderGraphBuffer(uint32_t size);
    ~RenderGraphBuffer();

    TB_INLINE RenderGraphResource::Type GetType() const override
    {
        return RenderGraphResource::Type::Buffer;
    }
};
