set(SRC GuidGen.cpp stdatl.cpp)
set(SRCH aboutdlg.h maindlg.h resource.h stdatl.h)
set(RC GuidGen.rc)
add_executable(GuidGen WIN32 ${SRC} ${RC})
