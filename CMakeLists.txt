cmake_minimum_required(VERSION 3.14)

project(example2 CXX)

include(FetchContent)
FetchContent_Declare(
  cmodule
  URL "https://github.com/scapix-com/cmodule/archive/v1.0.28.tar.gz"
  URL_HASH SHA256=5b8ae3f16de1b6e16d7802333beff618ebfe19e667d8244aa646d9d1c445fd6b
)
FetchContent_MakeAvailable(cmodule)

set(SCAPIX_BRIDGE "java" CACHE STRING "cpp, java, objc, python")
set(SCAPIX_JAVA_API "jdk-11.0.2" CACHE STRING "one of the folders inside 'scapix/java_api': jdk-11.0.2, android-28, etc.")

file(GLOB_RECURSE sources CONFIGURE_DEPENDS source/*)
source_group(TREE source PREFIX "source" FILES ${sources})

add_executable(example2 ${sources})

target_include_directories(example2 PRIVATE source)

find_package(JNI REQUIRED)
target_include_directories(example2 PUBLIC ${JNI_INCLUDE_DIRS})
target_link_libraries(example2 PUBLIC ${JNI_LIBRARIES})

# Windows: jvm.dll needs to be in PATH (copying jvm.dll doesn't work)
set_target_properties(example2 PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${JAVA_INCLUDE_PATH}/../jre/bin/server")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT example2)

find_package(Scapix REQUIRED)
target_link_libraries(example2 PRIVATE scapix)
