set(libs "Qt5::Widgets")

if(USE_ANN)
	list(APPEND libs "UANN_ann_static")
endif()

Ubpa_AddTarget(MODE "EXE" QT ON LIBS ${libs})
