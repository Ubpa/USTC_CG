Ubpa_GlobGroupSrcs(RST sources PATHS
	${CMAKE_CURRENT_SOURCE_DIR}
	"${PROJECT_SOURCE_DIR}/include/Basic"
)

Ubpa_AddTarget(MODE "LIB" SOURCES ${sources} LIBS Ubpa::UGM_core)
