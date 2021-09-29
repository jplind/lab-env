# assignment4
#--------------------------------------------------------------------------

PROJECT(assignment4)
FILE(GLOB assignment4_headers code/*.h)
FILE(GLOB assignment4_sources code/*.cc)

SET(files_assignment4 ${assignment4_headers} ${assignment4_sources})
SOURCE_GROUP("assignment4" FILES ${files_assignment4})

ADD_EXECUTABLE(assignment4 ${files_assignment4})
TARGET_LINK_LIBRARIES(assignment4 core render math)
ADD_DEPENDENCIES(assignment4 core render math)