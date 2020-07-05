Ubpa_DownloadZip(
	https://cdn.jsdelivr.net/gh/Ubpa/UData@master/common/eigen/eigen_3_3_7.zip
	eigen_3_3_7.zip
	SHA256 6D5CBC4C70A7B175439DEE0942C6794EB454493D3585973F7D4976D0820321E2
)

Ubpa_AddTarget(
  TEST
  MODE INTERFACE
  ADD_CURRENT_TO NONE
  INC
    "src/_deps/eigen"
)
