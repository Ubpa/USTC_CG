cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(ann_example VERSION 1.0)
message(STATUS "[Project] ${PROJECT_NAME}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()

Ubpa_AddDep(UANN 0.2.0)

Ubpa_AddSubDirsRec(src)
