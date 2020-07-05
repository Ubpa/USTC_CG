Ubpa_GetTargetName(sharedDArray "${PROJECT_SOURCE_DIR}/src/libraries/shared")
Ubpa_AddTarget(
  TEST
  MODE EXE
  LIB
    ${sharedDArray}
)
