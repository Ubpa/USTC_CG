Ubpa_GetTargetName(pathtracer ${PROJECT_SOURCE_DIR}/src/PathTracer)

Ubpa_AddTarget(MODE "EXE"
	LIBS Ubpa::UEngine_core ${pathtracer}
)
