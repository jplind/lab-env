# assignment3
#--------------------------------------------------------------------------

PROJECT(assignment3)
FILE(GLOB assignment3_headers code/*.h)
FILE(GLOB assignment3_sources code/*.cc)

SET(files_assignment3 ${assignment3_headers} ${assignment3_sources})
SOURCE_GROUP("assignment3" FILES ${files_assignment3})

ADD_EXECUTABLE(assignment3 ${files_assignment3})
TARGET_LINK_LIBRARIES(assignment3 core render math)
ADD_DEPENDENCIES(assignment3 core render math)