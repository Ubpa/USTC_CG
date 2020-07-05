Ubpa_GetTargetName(sharedPolynomialList "${PROJECT_SOURCE_DIR}/src/libraries/shared")
Ubpa_GetTargetName(staticPolynomialMap "${PROJECT_SOURCE_DIR}/src/libraries/static")
Ubpa_AddTarget(
  TEST
  MODE EXE
  LIB
    ${sharedPolynomialList}
    ${staticPolynomialMap}
)
