# assignment6
#--------------------------------------------------------------------------

PROJECT(assignment6)
FILE(GLOB assignment6_headers code/*.h)
FILE(GLOB assignment6_sources code/*.cc)

SET(files_assignment6 ${assignment6_headers} ${assignment6_sources})
SOURCE_GROUP("assignment6" FILES ${files_assignment6})

ADD_EXECUTABLE(assignment6 ${files_assignment6})
TARGET_LINK_LIBRARIES(assignment6 core render math)
ADD_DEPENDENCIES(assignment6 core render math)