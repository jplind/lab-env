#--------------------------------------------------------------------------
# math_test project
#--------------------------------------------------------------------------

PROJECT(math_test)
FILE(GLOB math_test_headers code/*.h)
FILE(GLOB math_test_sources code/*.cc)

SET(files_math_test ${math_test_headers} ${math_test_sources})
SOURCE_GROUP("math_test" FILES ${files_math_test})

ADD_EXECUTABLE(math_test ${files_math_test})
TARGET_LINK_LIBRARIES(math_test core render)
ADD_DEPENDENCIES(math_test core render)