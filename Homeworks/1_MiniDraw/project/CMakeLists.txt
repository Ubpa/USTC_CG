cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(MiniDraw VERSION 1.0)
message(STATUS "[Project] ${PROJECT_NAME}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()

Ubpa_QtInit(COMPONENTS Widgets)

Ubpa_AddSubDirsRec(src)
