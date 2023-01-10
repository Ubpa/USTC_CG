#项目名，默认为目录名
Ubpa_GetDirName(DIRNAME)
set(targetName "${TARGET_PREFIX}${DIRNAME}")
#多个源文件用 ';' 分隔
#如：set(sources "main.cpp;src_2.cpp;help.h")
file(GLOB sources
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
#多个库文件用 ';' 分隔，如果为空，则为 ""
#如：set(libs "targetName;xxx.lib")
set(libs "")
Ubpa_SetupTarget(MODE ${mode} NAME ${targetName} SOURCES ${sources} LIBS ${libs})
