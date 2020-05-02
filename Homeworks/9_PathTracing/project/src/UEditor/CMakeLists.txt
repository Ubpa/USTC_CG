Ubpa_GetTargetName(pathtracer ${PROJECT_SOURCE_DIR}/src/PathTracer)

Ubpa_AddTarget(
  MODE EXE
  LIB
    Ubpa::UEngine_core
    ${pathtracer}
)
