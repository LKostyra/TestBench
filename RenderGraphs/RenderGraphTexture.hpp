#pragma once

#include "RenderGraphResource.hpp"


class RenderGraphTexture: public RenderGraphResource
{
public:
    using Ptr = std::shared_ptr<RenderGraphTexture>;

private:
    uint32_t mWidth;
    uint32_t mHeight;

public:
    RenderGraphTexture(const std::string& name, uint32_t width, uint32_t height);
    ~RenderGraphTexture();

    TB_INLINE RenderGraphResource::Type GetResourceType() const override
    {
        return RenderGraphResource::Type::Texture;
    }

    TB_INLINE uint32_t GetWidth() const
    {
        return mWidth;
    }

    TB_INLINE uint32_t GetHeight() const
    {
        return mHeight;
    }
};
