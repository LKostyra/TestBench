SET(RENDER_GRAPHS_SRCS Main.cpp
                       RenderGraph.cpp
                       RenderGraphNode.cpp
                       RenderGraphPass.cpp
                       RenderGraphResource.cpp
                       RenderGraphBuffer.cpp
                       RenderGraphTexture.cpp
                       )

SET(RENDER_GRAPHS_HDRS RenderGraph.hpp
                       RenderGraphNode.hpp
                       RenderGraphPass.hpp
                       RenderGraphResource.hpp
                       RenderGraphBuffer.hpp
                       RenderGraphTexture.hpp
                       )

ADD_EXECUTABLE(${RENDER_GRAPHS_TARGET}
               ${RENDER_GRAPHS_SRCS} ${RENDER_GRAPHS_HDRS}
               )

ADD_DEPENDENCIES(${RENDER_GRAPHS_TARGET} lkCommon)

TARGET_INCLUDE_DIRECTORIES(${RENDER_GRAPHS_TARGET}
                           PUBLIC ${TESTBENCH_ROOT_DIRECTORY}/Deps/lkCommon)

TARGET_LINK_LIBRARIES(${RENDER_GRAPHS_TARGET} lkCommon)

TESTBENCH_COPY_RESULT(${RENDER_GRAPHS_TARGET})
