#pragma once

#include "RenderGraphResource.hpp"


class RenderGraphTexture: public RenderGraphResource
{
public:
    using Ptr = std::shared_ptr<RenderGraphTexture>;

private:
    uint32_t mSize;

public:
    RenderGraphTexture(uint32_t size);
    ~RenderGraphTexture();

    TB_INLINE RenderGraphResource::Type GetType() const override
    {
        return RenderGraphResource::Type::Texture;
    }
};
