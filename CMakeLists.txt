cmake_minimum_required(VERSION 3.20...3.27)

project(example2 CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS source/*)
source_group(TREE source PREFIX "source" FILES ${sources})

add_executable(example2 ${sources})
target_include_directories(example2 PRIVATE source)

include(FetchContent)
FetchContent_Declare(
  cmodule
  URL "https://github.com/scapix-com/cmodule/archive/refs/tags/v2.0.1.tar.gz"
  URL_HASH SHA256=a9477bbefb43b6fabdc2dc044207fe42cca63999c60b6baf06ba0c62fa116ec5
)
FetchContent_MakeAvailable(cmodule)

# optionally override API selection
# set(SCAPIX_JAVA_API "jdk-11" CACHE STRING "" FORCE)

find_package(ScapixJavaAPI REQUIRED)
target_link_libraries(example2 PRIVATE scapix::java_api)
scapix_java_api_source_group()

# VS debugger: add PATH to jvm.dll
find_path(JVM_DLL_PATH NAMES "jvm.dll" PATHS "${JAVA_INCLUDE_PATH}/../jre/bin/server" "${JAVA_INCLUDE_PATH}/../bin/server" NO_DEFAULT_PATH)
set_target_properties(example2 PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${JVM_DLL_PATH}")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT example2)
