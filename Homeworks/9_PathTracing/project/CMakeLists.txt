cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(HW9 VERSION 0.0.7)
message(STATUS "[Project] ${PROJECT_NAME}")

include(FetchContent)

message(STATUS "find package: UCMake 0.5.5")
find_package(UCMake 0.5.5 QUIET)
if(NOT UCMake_FOUND)
  message(STATUS "UCMake 0.5.5 not found")
  message(STATUS "fetch: ${_address} 0.5.5")
  set(_address "https://github.com/Ubpa/UCMake")
  FetchContent_Declare(
    UCMake
    GIT_REPOSITORY ${_address}
    GIT_TAG "0.5.5"
  )
  FetchContent_MakeAvailable(UCMake)
  message(STATUS "UCMake 0.5.5 build done")
endif()

Ubpa_InitProject()

Ubpa_AddDep(UEngine 0.0.7)

Ubpa_AddSubDirsRec(data)
Ubpa_AddSubDirsRec(src)
