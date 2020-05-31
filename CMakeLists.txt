cmake_minimum_required(VERSION 3.14)

project(example2 CXX)

include(FetchContent)
FetchContent_Declare(
  cmodule
  URL "https://github.com/scapix-com/cmodule/archive/v1.0.15.tar.gz"
  URL_HASH SHA256=e9809f4e30cff763ec0001d9b7d3292fc6cdc1c45edf2d8ad0c1603235300509
)
FetchContent_MakeAvailable(cmodule)

set(SCAPIX_BRIDGE "java" CACHE STRING "cpp, java, objc, python")
set(SCAPIX_JAVA_API "jdk-11.0.2" CACHE STRING "one of the folders inside 'scapix/java_api': jdk-11.0.2, android-28, etc.")

file(GLOB_RECURSE sources CONFIGURE_DEPENDS source/*.*)
source_group(TREE source PREFIX "source" FILES ${sources})

add_executable(example2 ${sources})

set_target_properties(example2 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON # not required, but speeds up compilation of SCAPIX_META_STRING with clang and gcc
)

target_include_directories(example2 PRIVATE source)

find_package(JNI REQUIRED)
target_include_directories(example2 PUBLIC ${JNI_INCLUDE_DIRS})
target_link_libraries(example2 PUBLIC ${JNI_LIBRARIES})

# Windows: jvm.dll needs to be in PATH (copying jvm.dll doesn't work)
set_target_properties(example2 PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${JAVA_INCLUDE_PATH}/../jre/bin/server")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT example2)

find_package(Scapix REQUIRED)
target_link_libraries(example2 PRIVATE scapix)
