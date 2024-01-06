
set(SDL2_image_ROOT "F:/C++Program/C++3rd/SDL2_image-devel-2.8.1-VC/SDL2_image-2.8.1" CACHE PATH "sdl2_image_root path (contain cmake)")

# view :http://cmake.org.cn/command/find_package.html#id7
# Search Modes ->Config Mode Search Procedure

find_package(SDL2_image CONFIG)

# 为什么??? 通过find_package 调用SDL2_image 就认了?!