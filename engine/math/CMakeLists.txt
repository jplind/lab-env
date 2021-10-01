#--------------------------------------------------------------------------
# math
#--------------------------------------------------------------------------

SET(files_math
	vec2.h
	vec3.h
	ivec3.h
	vec4.h
	mat4.h
	math.h
	math.cc)
SOURCE_GROUP("math" FILES ${files_math})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(math STATIC ${files_math} ${files_pch})
TARGET_PCH(math ../)
ADD_DEPENDENCIES(math glew)
TARGET_LINK_LIBRARIES(math PUBLIC engine exts glew)

FILE(GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
FOREACH(child ${children})
	IF(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${child})
		IF(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${child}/CMakeLists.txt)
			ADD_SUBDIRECTORY(${child})
		ENDIF()
	ENDIF()
ENDFOREACH()