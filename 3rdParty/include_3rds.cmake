
include(${CMAKE_CURRENT_LIST_DIR}/sdl/find_sdl2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/sdl2-image/find_sdl2_image.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/sdl2_ttf/find_SDL_ttf.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/sdl2_mixer/find_sdl2_mixer.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/sdl2_mixer/find_sdl2_mixer.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glm)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/spdlog) # spdlog 默认静态
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/stb_image) 
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/yaml)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ftxui)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs)


get_target_property(SDL2_image_DLL  SDL2_image::SDL2_image IMPORTED_LOCATION)
get_target_property(SDL2_DLL        SDL2::SDL2 IMPORTED_LOCATION)
get_target_property(SDL2_ttf_DLL    SDL2_ttf::SDL2_ttf IMPORTED_LOCATION)
get_target_property(SDL2_mixer_DLL  SDL2_mixer::SDL2_mixer IMPORTED_LOCATION)
get_target_property(glm_INCLUDE_DIR glm INTERFACE_INCLUDE_DIRECTORIES)

set(ftxui_LIBRARIES ftxui::screen ftxui::dom ftxui::component )
set(SDL_LIBRARIES  SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer ${SDL2_LIBRARIES})


# 对于子层cmake变量 设置cache使之上层可见
# 为了使目标在上层可见. -> 改为.cmake在top引用. cwd 成为顶层目录
set(3rd_LIBS        glm spdlog::spdlog stb_image yaml-cpp tinyfiledialogs ${ftxui_LIBRARIES} ${SDL_LIBRARIES} CACHE INTERNAL "3rd libs target names" )
set(3rd_LIBS_DLLS   ${SDL2_image_DLL}  ${SDL2_DLL}  ${SDL2_ttf_DLL} ${SDL2_mixer_DLL} CACHE INTERNAL "3rd libs' dll path lists" )
