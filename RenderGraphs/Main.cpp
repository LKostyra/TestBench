#include <Logger.hpp>
#include "RenderGraph.hpp"

int main()
{
    LOGI("Render Graphs test app");

    RenderGraph rg;

    // Simulate building a render graph below
    // loosely based on forward rendering plus with random values just to make sure it works

    // depth pass
    RenderGraphBuffer::Ptr vertexBuffer = std::make_shared<RenderGraphBuffer>(1024);
    RenderGraphTexture::Ptr depthTexture = std::make_shared<RenderGraphTexture>(1280, 720);
    RenderGraphPass::Ptr depthPass = std::make_shared<RenderGraphPass>(1280, 720);

    rg.AddRootPass(depthPass);
    RenderGraph::AddInputBufferToPass(depthPass, vertexBuffer);
    RenderGraph::AddOutputTextureToPass(depthPass, depthTexture);

    // compute pass
    RenderGraphBuffer::Ptr lightBuffer = std::make_shared<RenderGraphBuffer>(2048);
    RenderGraphBuffer::Ptr culledLightsBuffer = std::make_shared<RenderGraphBuffer>(512);
    RenderGraphPass::Ptr computePass = std::make_shared<RenderGraphPass>(1280, 720);

    RenderGraph::AddInputBufferToPass(computePass, lightBuffer);
    RenderGraph::AddInputTextureToPass(computePass, depthTexture);
    RenderGraph::AddOutputBufferToPass(computePass, culledLightsBuffer);

    // forward pass
    RenderGraphTexture::Ptr outputTex = std::make_shared<RenderGraphTexture>(1280, 720);
    RenderGraphPass::Ptr forwardPass = std::make_shared<RenderGraphPass>(1280, 720);

    RenderGraph::AddInputBufferToPass(forwardPass, culledLightsBuffer);
    RenderGraph::AddInputTextureToPass(forwardPass, depthTexture);
    RenderGraph::AddOutputTextureToPass(forwardPass, outputTex);

    // now the graph should be fully traversable

    return 0;
}
