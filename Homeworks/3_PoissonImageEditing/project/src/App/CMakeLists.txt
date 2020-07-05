Ubpa_GetTargetName(eigen "${PROJECT_SOURCE_DIR}/src/_deps/eigen")

Ubpa_AddTarget(
  TEST
  QT
  MODE EXE
  LIB
    ${eigen}
	Qt5::Widgets
	opencv_world
)
