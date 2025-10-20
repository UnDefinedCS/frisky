file(GLOB SRC cpp/*.cpp)
file(GLOB HPP hpp/*.hpp)

add_executable(tetris ${SRC} ${HPP})

# Raylib include and library paths
set(RAYLIB_DIR /opt/raylib-5.5_win64_mingw-w64)
target_include_directories(tetris PRIVATE
    ${RAYLIB_DIR}/include
)
target_link_directories(tetris PRIVATE
    ${RAYLIB_DIR}/lib
)

# Link libraries
target_link_libraries(tetris PRIVATE
    raylib
    opengl32
    winmm
)

# (mwindows) Avoid console window on Windows when running via Explorer
target_link_options(tetris PRIVATE -mwindows)
target_link_options(tetris PRIVATE -static)

target_compile_options(tetris PRIVATE
    -static-libgcc
    -static-libstdc++
    -mthreads
    -static
)