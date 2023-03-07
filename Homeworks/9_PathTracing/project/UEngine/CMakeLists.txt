cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UEngine VERSION 0.0.7)
message(STATUS "[Project] ${PROJECT_NAME}")

include(FetchContent)

message(STATUS "find package: UCMake 0.5.0")
find_package(UCMake 0.5.0 QUIET)
if(NOT UCMake_FOUND)
  message(STATUS "UCMake 0.5.0 not found")
  message(STATUS "fetch: ${_address} 0.5.0")
  set(_address "https://github.com/Ubpa/UCMake")
  FetchContent_Declare(
    UCMake
    GIT_REPOSITORY ${_address}
    GIT_TAG "0.5.0"
  )
  FetchContent_MakeAvailable(UCMake)
  message(STATUS "UCMake 0.5.0 build done")
endif()

Ubpa_DownloadZip(
	https://ubpa.github.io/UData/UEngine/imgui.zip
	imgui.zip
	SHA256 9E134D8DBE8FD94A292DDD313A80BA77C113CD4756B4429777169C147401E200
)

find_package(glfw3 REQUIRED)

Ubpa_AddDep(URTR 0.0.10)

Ubpa_AddSubDirsRec(src)

Ubpa_Export(DIRECTORIES "include")
