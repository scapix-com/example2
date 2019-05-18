cmake_minimum_required(VERSION 3.14)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
get_filename_component(PROJECT_ROOT ${PROJECT_ROOT} ABSOLUTE)

set(SCAPIX_ROOT ${PROJECT_ROOT}/../scapix CACHE STRING "scapix lib path")
get_filename_component(SCAPIX_ROOT ${SCAPIX_ROOT} ABSOLUTE)

if("$ENV{HUNTER_ROOT}" STREQUAL "")
    set(HUNTER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/build/.hunter")
endif()

include("${SCAPIX_ROOT}/project/cmake/modules/HunterGate.cmake")

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.171.tar.gz"
    SHA1 "5d68bcca78eee347239ca5f4d34f4b6c12683154"
)

project(example2 CXX)

set(SCAPIX_BRIDGE "java" CACHE STRING "cpp, java, objc, python")
set(SCAPIX_JAVA_API "jdk-11.0.2" CACHE STRING "one of the folders inside 'scapix/java_api': jdk-11.0.2, android-28, etc.")

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
	${PROJECT_ROOT}/source/*.*
)

add_executable(example2 ${sources})
source_group(TREE ${PROJECT_ROOT}/source PREFIX "source" FILES ${sources})

set_target_properties(example2 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON # not required, but speeds up compilation of SCAPIX_META_STRING with clang and gcc
)

target_include_directories(example2 PRIVATE ${PROJECT_ROOT}/source)
target_compile_definitions(example2 PUBLIC SCAPIX_BRIDGE=${SCAPIX_BRIDGE})

add_subdirectory(${SCAPIX_ROOT}/project/cmake ${PROJECT_BINARY_DIR}/scapix)
target_link_libraries(example2 PRIVATE scapix)
