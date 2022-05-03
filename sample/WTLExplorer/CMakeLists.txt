set(SRC stdafx.cpp WTLExplorer.cpp MainFrm.cpp ShellMgr.cpp)
set(RC WTLExplorer.rc)
add_executable(WTLExplorer WIN32 ${SRC} ${RC})
