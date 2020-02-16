#Ubpa_QtBegin()

Ubpa_GlobGroupSrcs(RST sources PATHS
	${CMAKE_CURRENT_SOURCE_DIR}
	"${PROJECT_SOURCE_DIR}/include/test_02_lib"
)

file(GLOB qtFiles
	"${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
)

list(APPEND sources "${qtFiles}")

Ubpa_AddTarget(MODE "EXE" QT ON SOURCES ${sources} LIBS "Qt5::Widgets")

#Ubpa_QtEnd()