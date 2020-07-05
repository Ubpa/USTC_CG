cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(PoissonImageEditing VERSION 1.0)
message(STATUS "[Project] ${PROJECT_NAME}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()

Ubpa_QtInit(COMPONENTS Core Gui Widgets)

find_package(OpenCV REQUIRED)

Ubpa_AddSubDirsRec(src)
