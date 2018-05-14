#pragma once

#include "RenderGraphResource.hpp"


class RenderGraphBuffer: public RenderGraphResource
{
public:
    using Ptr = std::shared_ptr<RenderGraphBuffer>;

private:
    uint32_t mSize;

public:
    RenderGraphBuffer(const std::string& name, uint32_t size);
    ~RenderGraphBuffer();

    LKCOMMON_INLINE RenderGraphResource::Type GetResourceType() const override
    {
        return RenderGraphResource::Type::Buffer;
    }

    LKCOMMON_INLINE uint32_t GetSize() const
    {
        return mSize;
    }
};
