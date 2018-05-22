#include "RenderGraphNode.hpp"


RenderGraphNode::RenderGraphNode(const std::string& name)
    : mName(name)
    , mTraverseFlag(false)
{
}

RenderGraphNode::~RenderGraphNode()
{
}
