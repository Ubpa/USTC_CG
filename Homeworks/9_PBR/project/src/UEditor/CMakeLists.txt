Ubpa_GetTargetName(pathtracer ${PROJECT_SOURCE_DIR}/src/PathTracer)

Ubpa_AddTarget(MODE "EXE"
	SOURCES ${sources}
	LIBS Ubpa::UEngine_core ${pathtracer}
)
