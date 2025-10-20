file(GLOB SRC cpp/*.cpp)
file(GLOB HPP hpp/*.hpp)

add_executable(frisky ${SRC} ${HPP})

# Link libraries
target_link_libraries(frisky PRIVATE
    gdi32
    ws2_32
)

# (mwindows) Avoid console window on Windows when running via Explorer
target_link_options(frisky PRIVATE -mwindows)
target_link_options(frisky PRIVATE -static)

target_compile_options(frisky PRIVATE
    -static-libgcc
    -static-libstdc++
    -mthreads
    -static
)