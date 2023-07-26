cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(
	Sylvia
	VERSION 0.0.0
	DESCRIPTION "An easy to use, general-purpose, functional programming language"
	LANGUAGES CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard to use")
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)

	file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
	if(EXISTS "${LOC_PATH}")
		message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
	endif()

	include(CheckIPOSupported)
	check_ipo_supported(RESULT IPO_SUPPORTED)
	if(IPO_SUPPORTED)
		message(STATUS "Interprocedural optimization supported")
	else()
		message(STATUS "Interprocedural optimization not supported")
	endif()

	if(MSVC)
		add_compile_options(/W4 /WX)
	else()
		add_compile_options(-Wall -Wextra -pedantic -Werror)
	endif()
endif()

set(SYLVIA_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB SYLVIA_SOURCES "${SYLVIA_SRC_DIR}/*.cpp" "${SYLVIA_SRC_DIR}/**/*.cpp")
file(GLOB SYLVIA_TESTS "${SYLVIA_SRC_DIR}/*_test.cpp" "${SYLVIA_SRC_DIR}/**/*_test.cpp")
list(REMOVE_ITEM SYLVIA_SOURCES "${SYLVIA_SRC_DIR}/main.cpp" "${SYLVIA_TESTS}")

add_executable(sylvia "${SYLVIA_SRC_DIR}/main.cpp" "${SYLVIA_SOURCES}")
target_include_directories(sylvia PRIVATE "${SYLVIA_SRC_DIR}")
if(IPO_SUPPORTED)
  set_target_properties(sylvia PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

install(TARGETS sylvia DESTINATION bin)
