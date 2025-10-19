file(GLOB SRC cpp/*.cpp)
file(GLOB HPP hpp/*.hpp)

file(GLOB GAME_SRC game/*.cpp)
file(GLOB GAME_HPP game/*.hpp)

add_executable(frisky ${SRC} ${HPP} ${GAME_SRC} ${GAME_HPP})

target_compile_options(frisky PRIVATE
    -static-libgcc
    -static-libstdc++
    -static
    -mthreads
)
target_link_options(frisky PRIVATE -static)

# --- Raylib include and library paths ---
set(RAYLIB_DIR /opt/raylib-5.5_win64_mingw-w64)

target_include_directories(frisky PRIVATE
    ${RAYLIB_DIR}/include
)

target_link_directories(frisky PRIVATE
    ${RAYLIB_DIR}/lib
)

# Link libraries
target_link_libraries(frisky PRIVATE
    raylib
    opengl32
    gdi32
    winmm
    ws2_32
)

# Avoid console window on Windows when running via Explorer
target_link_options(frisky PRIVATE -mwindows)