Ubpa_GetTargetName(staticDArray "${PROJECT_SOURCE_DIR}/src/libraries/static")
Ubpa_AddTarget(
  TEST
  MODE EXE
  LIB
    ${staticDArray}
)
