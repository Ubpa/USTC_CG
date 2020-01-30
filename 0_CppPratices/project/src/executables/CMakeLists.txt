#生成 exe 就 "EXE"，生成 lib 就 "LIB"，生成 dll 就 "DLL"
set(mode "EXE")
set(TARGET_PREFIX "")

Ubpa_GetDirName(DIRNAME)
set(FOLDER_NAME ${DIRNAME})

Ubpa_AddSubDirs()