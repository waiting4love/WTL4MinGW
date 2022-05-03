set(SRC Alpha.cpp stdafx.cpp)
set(SRCH mainfrm.h resource.h stdafx.h Alpha.h view.h)
set(RC Alpha.rc)
add_executable(alpha WIN32 ${SRC} ${RC})