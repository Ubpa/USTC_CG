Ubpa_GetTargetName(tool "${PROJECT_SOURCE_DIR}/src/tool")
set(src
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${PROJECT_SOURCE_DIR}/data/shaders/dn.vert
	${PROJECT_SOURCE_DIR}/data/shaders/light_dn.frag)
Ubpa_AddTarget(MODE "EXE" SOURCES ${src} LIBS "Ubpa::UGL_core;glfw;${tool}")
