#--------------------------------------------------------------------------
# core
#--------------------------------------------------------------------------

SET(files_core
	app.h
	app.cc)
SOURCE_GROUP("core" FILES ${files_core})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(core STATIC ${files_core} ${files_pch})
TARGET_PCH(core ../)
ADD_DEPENDENCIES(core glew)
TARGET_LINK_LIBRARIES(core PUBLIC engine exts glew)

FILE(GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
FOREACH(child ${children})
	IF(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${child})
		IF(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${child}/CMakeLists.txt)
			ADD_SUBDIRECTORY(${child})
		ENDIF()
	ENDIF()
ENDFOREACH()