cmake_minimum_required(VERSION 3.0.0)
project(wtl4gcc VERSION 0.1.0)

include_directories(
    atlport/include
    stdext/include
    wtl/include
    )
link_libraries(kernel32 user32 gdi32 comdlg32 ole32 winspool shlwapi comctl32)
if(MINGW)
    add_compile_options(-Wno-narrowing -fpermissive -w)
endif()
add_subdirectory(sample/Alpha)
add_subdirectory(sample/BmpView)
add_subdirectory(sample/GuidGen)
add_subdirectory(sample/MDIDocVw)
add_subdirectory(sample/MTPad)
add_subdirectory(sample/Wizard97Test)
add_subdirectory(sample/WTLExplorer)