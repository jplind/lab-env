#--------------------------------------------------------------------------
# graphicsNodeTest project
#--------------------------------------------------------------------------

PROJECT(graphicsNodeTest)
FILE(GLOB graphicsNodeTest_headers code/*.h)
FILE(GLOB graphicsNodeTest_sources code/*.cc)

SET(files_graphicsNodeTest ${graphicsNodeTest_headers} ${graphicsNodeTest_sources})
SOURCE_GROUP("graphicsNodeTest" FILES ${files_graphicsNodeTest})

ADD_EXECUTABLE(graphicsNodeTest ${files_graphicsNodeTest})
TARGET_LINK_LIBRARIES(graphicsNodeTest core render math)
ADD_DEPENDENCIES(graphicsNodeTest core render math)