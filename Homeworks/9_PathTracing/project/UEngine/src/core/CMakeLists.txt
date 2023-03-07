Ubpa_GetTargetName(imgui "${PROJECT_SOURCE_DIR}/src/_deps/imgui")
Ubpa_GetTargetName(imgui_impl "${PROJECT_SOURCE_DIR}/src/_deps/imgui_impl")

Ubpa_AddTarget(
  MODE STATIC
  SOURCE
    "${PROJECT_SOURCE_DIR}/include/UEngine"
  LIB
    glfw
    ${imgui}
    Ubpa::URTR_core
  LIB_PRIVATE
    ${imgui_impl}
  INC
    include
)
