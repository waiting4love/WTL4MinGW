set(SRC BmpView.cpp stdafx.cpp)
set(SRCH list.h mainfrm.h props.h resource.h stdafx.h view.h)
set(RC BmpView.rc)
add_executable(bmpview WIN32 ${SRC} ${RC})
