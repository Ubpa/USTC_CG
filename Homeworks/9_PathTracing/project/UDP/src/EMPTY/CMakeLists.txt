Ubpa_GetTargetName(core "${PROJECT_SOURCE_DIR}/src/core")
Ubpa_AddTarget(
  TEST
  MODE EXE
  SOURCE
    "${PROJECT_SOURCE_DIR}/include/UDP"
  LIB
    ${core}
)
