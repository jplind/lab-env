#--------------------------------------------------------------------------
# texturesAndCameraTest project
#--------------------------------------------------------------------------

PROJECT(texturesAndCameraTest)
FILE(GLOB texturesAndCameraTest_headers code/*.h)
FILE(GLOB texturesAndCameraTest_sources code/*.cc)

SET(files_texturesAndCameraTest ${texturesAndCameraTest_headers} ${texturesAndCameraTest_sources})
SOURCE_GROUP("texturesAndCameraTest" FILES ${files_texturesAndCameraTest})

ADD_EXECUTABLE(texturesAndCameraTest ${files_texturesAndCameraTest})
TARGET_LINK_LIBRARIES(texturesAndCameraTest core render)
ADD_DEPENDENCIES(texturesAndCameraTest core render)