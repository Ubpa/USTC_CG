set(c_options "")
if(MSVC)
  list(APPEND c_options "/wd4251")
endif()

Ubpa_AddTarget(
  TEST
  MODE SHARED
  INC
    "include"
  DEFINE
    Ubpa_AsShared_PolynomialList
  DEFINE_PRIVATE
	Ubpa_Export_PolynomialList
  C_OPTION
    ${c_options}
  SOURCE
    "${PROJECT_SOURCE_DIR}/include/PolynomialList.h"
)
