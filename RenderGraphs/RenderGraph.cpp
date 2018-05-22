#include "RenderGraph.hpp"
#include <lkCommon/Utils/Logger.hpp>

#include <queue>


RenderGraph::RenderGraph()
{
}

RenderGraph::~RenderGraph()
{
}

void RenderGraph::PrintNode(const RenderGraphNode::Ptr& node)
{
    LOGI("    Node name: " << node->mName);
    switch (node->GetType())
    {
    case RenderGraphNode::Type::Resource:
    {
        RenderGraphResource::Ptr res = std::dynamic_pointer_cast<RenderGraphResource>(node);

        LOGI("      Type: Resource");
        switch (res->GetResourceType())
        {
        case RenderGraphResource::Type::Buffer:
        {
            RenderGraphBuffer::Ptr buffer = std::dynamic_pointer_cast<RenderGraphBuffer>(res);
            LOGI("      Resource Type: Buffer");
            LOGI("        Size: " << buffer->GetSize());
            break;
        }
        case RenderGraphResource::Type::Texture:
        {
            RenderGraphTexture::Ptr tex = std::dynamic_pointer_cast<RenderGraphTexture>(res);
            LOGI("      Resource Type: Texture");
            LOGI("        Width:  " << tex->GetWidth());
            LOGI("        Height: " << tex->GetHeight());
            break;
        }
        default:
        {
            LOGI("      Resource Type: Unknown");
        }
        }

        break;
    }
    default:
    {
        LOGI("      Type: Unknown");
    }
    }
}

void RenderGraph::PrintPassNode(const RenderGraphNode::Ptr& node)
{
    if (node->GetType() != RenderGraphNode::Type::Pass)
        return;

    RenderGraphPass::Ptr pass = std::dynamic_pointer_cast<RenderGraphPass>(node);

    LOGW("Render Graph pass node " << node->mName << ":");
    LOGI("  Width:  " << pass->GetWidth());
    LOGI("  Height: " << pass->GetHeight());

    LOGI("  Inputs:");
    for (auto& n: pass->mInputs)
    {
        PrintNode(n);
    }

    LOGI("  Outputs:");
    for (auto& n: pass->mOutputs)
    {
        PrintNode(n);
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
        rn->mTraverseFlag ^= true;
    }

    if (nodes.empty())
        return;

    bool currentTraverseFlag = !nodes.front()->mTraverseFlag;

    while (!nodes.empty())
    {
        RenderGraphNode::Ptr node = nodes.front();

        for (auto& n: node->mOutputs)
        {
            if (n->mTraverseFlag == currentTraverseFlag)
            {
                nodes.push(n);
                n->mTraverseFlag ^= true;
            }
        }

        PrintPassNode(node);

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
