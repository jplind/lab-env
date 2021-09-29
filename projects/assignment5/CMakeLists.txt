# assignment5
#--------------------------------------------------------------------------

PROJECT(assignment5)
FILE(GLOB assignment5_headers code/*.h)
FILE(GLOB assignment5_sources code/*.cc)

SET(files_assignment5 ${assignment5_headers} ${assignment5_sources})
SOURCE_GROUP("assignment5" FILES ${files_assignment5})

ADD_EXECUTABLE(assignment5 ${files_assignment5})
TARGET_LINK_LIBRARIES(assignment5 core render math)
ADD_DEPENDENCIES(assignment5 core render math)