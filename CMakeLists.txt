cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

include(FetchContent)

project(HyperionEvents VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

#############################################################################
# Import GoogleTest
#############################################################################
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	${CMAKE_CURRENT_BINARY_DIR}/googletest-build
	EXCLUDE_FROM_ALL)
#############################################################################
#############################################################################


#############################################################################
# Import Microsoft GSL Implementation
#############################################################################
FetchContent_Declare(GSL
	GIT_REPOSITORY "https://github.com/microsoft/GSL"
	GIT_TAG "v3.1.0"
	)

FetchContent_MakeAvailable(GSL)

add_library(HyperionEvents INTERFACE)

if(MSVC)
	target_compile_options(HyperionEvents PRIVATE /WX /W4 /std:c++20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "clang")
	if(NOT WIN32)
		target_compile_options(HyperionEvents PRIVATE
			-std=c++20
			-Wall
			-Wextra
			-Wpedantic
			-Weverything
			-Werror
			-Wno-c++98-compat
			-Wno-c++98-compat-pedantic
			-Wno-c++98-c++11-c++14-compat-pedantic
			-Wno-c++20-compat
			)
	else()
		target_compile_options(HyperionEvents PRIVATE
			-std=c++20
			-Wall
			-Wextra
			-Wpedantic
			-Weverything
			-Werror
			-Wno-c++98-compat
			-Wno-c++98-compat-pedantic
			-Wno-c++98-c++11-c++14-compat-pedantic
			-Wno-c++20-compat
			)
	endif()
else()
	target_compile_options(HyperionEvents PRIVATE
		-std=c++20
		-Wall
		-Wextra
		-Wpedantic
		-Werror
		-Wno-c++98-compat
		-Wno-c++98-compat-pedantic
		-Wno-c++98-c++11-c++14-compat-pedantic
		-Wno-c++20-compat
		)
endif()

target_include_directories(HyperionEvents BEFORE PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/src/events"
	"${CMAKE_SOURCE_DIR}/src/signals"
	BEFORE INTERFACE
	"${CMAKE_SOURCE_DIR}/include"
	)

###### We add headers to sources sets because it helps with `#include` lookup for some tooling #####

set(EVENTS
	"${CMAKE_SOURCE_DIR}/src/events/EventsInternal.h"
	)

set(SIGNALS
	"${CMAKE_SOURCE_DIR}/src/signals/SignalsInternal.h"
	)

set(EXPORTS
	"${CMAKE_SOURCE_DIR}/include/Events.h"
	"${CMAKE_SOURCE_DIR}/include/Signals.h"
	"${CMAKE_SOURCE_DIR}/include/HyperionEvents.h"
	)

target_sources(HyperionEvents PRIVATE
	${EVENTS}
	${SIGNALS}
	INTERFACE
	${EXPORTS}
	)

target_link_libraries(HyperionEvents INTERFACE
	GSL)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "clang" OR APPLE)
	set_target_properties(HyperionEvents PROPERTIES CXX_CLANG_TIDY clang-tidy)
endif()


add_executable(HyperionEventsTest "${CMAKE_SOURCE_DIR}/src/test/HyperionEventsTest.cpp")

if(MSVC)
	target_compile_options(HyperionEventsTest PRIVATE /WX /W4 /std:c++20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "clang")
	if(NOT WIN32)
		target_compile_options(HyperionEventsTest PRIVATE
			-std=c++20
			-Wall
			-Wextra
			-Wpedantic
			-Weverything
			-Werror
			-Wno-c++98-compat
			-Wno-c++98-compat-pedantic
			-Wno-c++98-c++11-c++14-compat-pedantic
			-Wno-c++20-compat
			-Wno-global-constructors
			)
	else()
		target_compile_options(HyperionEventsTest PRIVATE
			-std=c++20
			-Wall
			-Wextra
			-Wpedantic
			-Weverything
			-Werror
			-Wno-c++98-compat
			-Wno-c++98-compat-pedantic
			-Wno-c++98-c++11-c++14-compat-pedantic
			-Wno-c++20-compat
			-Wno-global-constructors
			)
	endif()
else()
	target_compile_options(HyperionEventsTest PRIVATE
		-std=c++20
		-Wall
		-Wextra
		-Wpedantic
		-Werror
		-Wno-c++98-compat
		-Wno-c++98-compat-pedantic
		-Wno-c++98-c++11-c++14-compat-pedantic
		-Wno-c++20-compat
		-Wno-global-constructors
		)
endif()

target_include_directories(HyperionEventsTest BEFORE INTERFACE
	"${CMAKE_SOURCE_DIR}/src/test"
	"${CMAKE_SOURCE_DIR}/src/events/test"
	"${CMAKE_SOURCE_DIR}/src/signals/test"
	)

if(UNIX AND NOT APPLE)
	target_link_libraries(HyperionEventsTest PRIVATE
		curl
		GSL
		gtest)
else()
	target_link_libraries(HyperionEventsTest PRIVATE
		GSL
		gtest)
endif()
