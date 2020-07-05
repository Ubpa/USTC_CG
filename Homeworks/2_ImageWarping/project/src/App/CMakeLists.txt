Ubpa_GetTargetName(eigen "${PROJECT_SOURCE_DIR}/src/_deps/eigen")

set(libs "${eigen};Qt5::Widgets")

if(USE_ANN)
	list(APPEND libs "Ubpa::UANN_ann")
endif()

Ubpa_AddTarget(
  TEST
  QT
  MODE EXE
  LIB
    ${libs}
)
