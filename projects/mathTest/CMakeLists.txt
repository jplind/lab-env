#--------------------------------------------------------------------------
# math-lib
#--------------------------------------------------------------------------
PROJECT(math-lib)
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

####################################
# Add your class files in this list:
SET(files
		main.cc
		util.h
		vec4.h
		vec3.h
		mat4.h
	)
####################################

if(MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
ENDIF()
IF(APPLE)
	SET(CMAKE_CXX_FLAGS "-g -std=c++17 -stdlib=libc++")
ENDIF()

SET(ENV_ROOT ${CMAKE_CURRENT_DIR})

if (MSVC)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/bin>)
else()
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

SOURCE_GROUP("math-lib" FILES ${files})

ADD_EXECUTABLE(math-lib ${files})
