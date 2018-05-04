#include "RenderGraphNode.hpp"


RenderGraphNode::RenderGraphNode()
{
}

RenderGraphNode::~RenderGraphNode()
{
}

void RenderGraphNode::AddInput(RenderGraphNode::Ptr& ptr)
{
    mInputs.push_back(ptr);
}

void RenderGraphNode::AddOutput(RenderGraphNode::Ptr& ptr)
{
    mOutputs.push_back(ptr);
}
