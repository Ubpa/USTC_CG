set(src
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${PROJECT_SOURCE_DIR}/data/shaders/p3t2n3.vert
	${PROJECT_SOURCE_DIR}/data/shaders/p3.vert
	${PROJECT_SOURCE_DIR}/data/shaders/light_shadow.frag
	${PROJECT_SOURCE_DIR}/data/shaders/empty.frag
)

Ubpa_GetTargetName(tool "${PROJECT_SOURCE_DIR}/src/tool")
Ubpa_AddTarget(MODE "EXE" SOURCES ${src} LIBS "Ubpa::UGL_core;glfw;${tool}")
