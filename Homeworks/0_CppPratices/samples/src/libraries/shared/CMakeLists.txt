Ubpa_AddTarget(
  TEST
  MODE SHARED
  INC
    "include"
  DEFINE
    Ubpa_AsShared_DArray
  DEFINE_PRIVATE
	Ubpa_Export_DArray
  SOURCE
    "${PROJECT_SOURCE_DIR}/include/DArray.h"
	"${PROJECT_SOURCE_DIR}/src/executables/2_EfficientDArray/DArray.cpp"
)
