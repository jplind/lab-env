# test
#--------------------------------------------------------------------------

PROJECT(test)
FILE(GLOB test_headers code/*.h)
FILE(GLOB test_sources code/*.cc)

SET(files_test ${test_headers} ${test_sources})
SOURCE_GROUP("test" FILES ${files_test})

ADD_EXECUTABLE(test ${files_test})
TARGET_LINK_LIBRARIES(test core render math)
ADD_DEPENDENCIES(test core render math)