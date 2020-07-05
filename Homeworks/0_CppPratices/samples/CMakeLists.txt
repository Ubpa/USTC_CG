cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(CppPractices VERSION 1.0)
message(STATUS "[Project] ${PROJECT_NAME} ${PROJECT_VERSION}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()

Ubpa_AddSubDirsRec(src)
