Ubpa_GetTargetName(eigen "${PROJECT_SOURCE_DIR}/src/_deps/eigen")

Ubpa_AddTarget(
  TEST
  MODE EXE
  LIB
    ${eigen}
)
