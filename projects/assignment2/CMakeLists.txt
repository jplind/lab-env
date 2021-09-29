#--------------------------------------------------------------------------
# assignment2
#--------------------------------------------------------------------------

PROJECT(assignment2)
FILE(GLOB assignment2_headers code/*.h)
FILE(GLOB assignment2_sources code/*.cc)

SET(files_assignment2 ${assignment2_headers} ${assignment2_sources})
SOURCE_GROUP("assignment2" FILES ${files_assignment2})

ADD_EXECUTABLE(assignment2 ${files_assignment2})
TARGET_LINK_LIBRARIES(assignment2 core render math)
ADD_DEPENDENCIES(assignment2 core render math)