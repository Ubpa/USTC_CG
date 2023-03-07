cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(HW9 VERSION 0.0.7)
message(STATUS "[Project] ${PROJECT_NAME}")

set(CMAKE_DEBUG_POSTFIX "d")

include(FetchContent)

message(STATUS "find package: UCMake 0.5.3")
find_package(UCMake 0.5.3 QUIET)
if(NOT UCMake_FOUND)
  message(STATUS "UCMake 0.5.3 not found")
  message(STATUS "fetch: UCMake 0.5.3")
  set(_address "https://github.com/Ubpa/UCMake")
  FetchContent_Declare(
    UCMake
    GIT_REPOSITORY ${_address}
    GIT_TAG "0.5.3"
  )
  FetchContent_MakeAvailable(UCMake)
  message(STATUS "UCMake 0.5.3 build done")
endif()

Ubpa_InitProject()

Ubpa_AddDep(UEngine 0.0.7)

Ubpa_AddSubDirsRec(data)
Ubpa_AddSubDirsRec(src)
