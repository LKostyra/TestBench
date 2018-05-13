#include "RenderGraph.hpp"
#include "Logger.hpp"

#include <queue>


RenderGraph::RenderGraph()
{
}

RenderGraph::~RenderGraph()
{
}

void RenderGraph::PrintNode(const RenderGraphNode::Ptr& node)
{
    LOGI("Render Graph node " << node->mName << ":");

    switch (node->GetType())
    {
    case RenderGraphNode::Type::Pass:
    {
        RenderGraphPass::Ptr pass = std::dynamic_pointer_cast<RenderGraphPass>(node);
        LOGI("  Type: Pass");
        LOGI("    Width:  " << pass->GetWidth());
        LOGI("    Height: " << pass->GetHeight());
        break;
    }
    case RenderGraphNode::Type::Resource:
    {
        RenderGraphResource::Ptr res = std::dynamic_pointer_cast<RenderGraphResource>(node);

        LOGI("  Type: Resource");
        switch (res->GetResourceType())
        {
        case RenderGraphResource::Type::Buffer:
        {
            RenderGraphBuffer::Ptr buffer = std::dynamic_pointer_cast<RenderGraphBuffer>(res);
            LOGI("  Resource Type: Buffer");
            LOGI("    Size: " << buffer->GetSize());
            break;
        }
        case RenderGraphResource::Type::Texture:
        {
            RenderGraphTexture::Ptr tex = std::dynamic_pointer_cast<RenderGraphTexture>(res);
            LOGI("  Resource Type: Texture");
            LOGI("    Width:  " << tex->GetWidth());
            LOGI("    Height: " << tex->GetHeight());
            break;
        }
        default:
        {
            LOGI("  Resource Type: Unknown");
        }
        }

        break;
    }
    default:
    {
        LOGI("  Type: Unknown");
    }
    }
}

void RenderGraph::AddRootPass(RenderGraphPass::Ptr& pass)
{
    mRootPasses.push_back(pass);
}

void RenderGraph::PrintGraph()
{
    std::queue<RenderGraphNode::Ptr> nodes;

    for (auto& rn: mRootPasses)
    {
        nodes.push(rn);
    }

    while (!nodes.empty())
    {
        RenderGraphNode::Ptr node = nodes.front();

        for (auto& n: node->mOutputs)
        {
            nodes.push(n);
        }

        PrintNode(node);

        nodes.pop();
    }
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
