set(src
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${PROJECT_SOURCE_DIR}/data/shaders/p3t2n3_denoise.vert
	${PROJECT_SOURCE_DIR}/data/shaders/light.frag)

Ubpa_GetTargetName(tool "${PROJECT_SOURCE_DIR}/src/tool")
Ubpa_AddTarget(MODE "EXE" SOURCES ${src} LIBS "Ubpa::UGL_core;glfw;${tool}")
