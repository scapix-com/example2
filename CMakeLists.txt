cmake_minimum_required(VERSION 3.14...3.26)

project(example2 CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS source/*)
source_group(TREE source PREFIX "source" FILES ${sources})

add_executable(example2 ${sources})
target_include_directories(example2 PRIVATE source)

include(FetchContent)
FetchContent_Declare(
  cmodule
  URL "https://github.com/scapix-com/cmodule/archive/refs/tags/v1.0.46.tar.gz"
  URL_HASH SHA256=d9a76c1e3ff35a4172da18985538a4f3e76f6b92c4d7458b0cdfbdc0bbf78264
)
FetchContent_MakeAvailable(cmodule)

set(SCAPIX_BRIDGE "java" CACHE STRING "")
set(SCAPIX_JAVA_API "jdk-11.0.2" CACHE STRING "")

find_package(Scapix REQUIRED)
target_compile_features(scapix PUBLIC cxx_std_20)
target_link_libraries(example2 PRIVATE scapix)

# Windows: jvm.dll needs to be in PATH (copying jvm.dll doesn't work)
find_path(JVM_DLL_PATH NAMES "jvm.dll" PATHS "${JAVA_INCLUDE_PATH}/../jre/bin/server" "${JAVA_INCLUDE_PATH}/../bin/server" NO_DEFAULT_PATH)
set_target_properties(example2 PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${JVM_DLL_PATH}")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT example2)
