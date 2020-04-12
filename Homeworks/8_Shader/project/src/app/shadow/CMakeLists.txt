Ubpa_GetTargetName(tool "${PROJECT_SOURCE_DIR}/src/tool")
Ubpa_AddTarget(MODE "EXE" LIBS "Ubpa::UGL_core;glfw;${tool}")
