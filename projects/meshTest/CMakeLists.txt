#--------------------------------------------------------------------------
# meshTest project
#--------------------------------------------------------------------------

PROJECT(meshTest)
FILE(GLOB meshTest_headers code/*.h)
FILE(GLOB meshTest_sources code/*.cc)

SET(files_meshTest ${meshTest_headers} ${meshTest_sources})
SOURCE_GROUP("meshTest" FILES ${files_meshTest})

ADD_EXECUTABLE(meshTest ${files_meshTest})
TARGET_LINK_LIBRARIES(meshTest core render)
ADD_DEPENDENCIES(meshTest core render)