Ubpa_GetTargetName(camera "${PROJECT_SOURCE_DIR}/src/camera")
Ubpa_AddTarget(MODE "EXE" LIBS "Ubpa::UGL_core;glfw;${camera}")
