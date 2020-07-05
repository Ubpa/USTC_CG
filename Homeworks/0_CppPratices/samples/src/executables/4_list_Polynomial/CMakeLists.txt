Ubpa_GetTargetName(sharedPolynomialList "${PROJECT_SOURCE_DIR}/src/libraries/shared")
Ubpa_AddTarget(
  TEST
  MODE EXE
  LIB
    ${sharedPolynomialList}
)
