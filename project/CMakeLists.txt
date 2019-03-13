cmake_minimum_required(VERSION 3.12)

project(example2)

set(scapix_bridge "java" CACHE STRING "cpp, java, objc, python")
set(scapix_java_api "jdk-11.0.2" CACHE STRING "one of the folders inside 'scapix/java_api': jdk-11.0.2, android-28, etc.")

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
get_filename_component(PROJECT_ROOT ${PROJECT_ROOT} ABSOLUTE)

set(SCAPIX_ROOT ${PROJECT_ROOT}/../scapix)
get_filename_component(SCAPIX_ROOT ${SCAPIX_ROOT} ABSOLUTE)

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

set_target_properties(example2 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(example2 PRIVATE ${PROJECT_ROOT}/source)
target_compile_definitions(example2 PUBLIC SCAPIX_BRIDGE=${scapix_bridge})

add_subdirectory(${SCAPIX_ROOT}/project/cmake ${PROJECT_BINARY_DIR}/scapix)
target_link_libraries(example2 PRIVATE scapix)
