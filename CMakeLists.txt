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
# Import Microsoft GSL Implementation
#############################################################################
FetchContent_Declare(GSL
	GIT_REPOSITORY "https://github.com/microsoft/GSL"
	GIT_TAG "v3.1.0"
	)

FetchContent_MakeAvailable(GSL)
#############################################################################
#############################################################################

add_library(HyperionEvents INTERFACE)

if(MSVC)
	target_compile_options(HyperionEvents PRIVATE /WX /W4 /std:c++20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "clang")
	if(NOT WIN32)
		target_compile_options(HyperionEvents INTERFACE
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
		target_compile_options(HyperionEvents INTERFACE
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
	target_compile_options(HyperionEvents INTERFACE
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

target_include_directories(HyperionEvents BEFORE INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	)

###### We add headers to sources sets because it helps with `#include` lookup for some tooling #####

set(EVENTS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/events/EventsInternal.h"
	)

set(SIGNALS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/signals/SignalsInternal.h"
	)

set(EXPORTS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/HyperionEvents/Events.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/HyperionEvents/Signals.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/HyperionEvents/HyperionEvents.h"
	)

target_sources(HyperionEvents INTERFACE
	${EVENTS}
	${SIGNALS}
	${EXPORTS}
	)

target_link_libraries(HyperionEvents INTERFACE
	GSL)

