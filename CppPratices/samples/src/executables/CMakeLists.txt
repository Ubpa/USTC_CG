#生成 exe 就 "EXE"，生成 lib 就 "LIB"
set(mode "EXE")
set(TARGET_PREFIX "")

Ubpa_GetDirName(DIRNAME)
set(FOLDER_NAME ${DIRNAME})

Ubpa_AddSubDirs()