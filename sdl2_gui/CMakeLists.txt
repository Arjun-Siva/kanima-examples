#add_executable(sdl2_gui_test
#    main.cpp
#    sdl_gui.cpp
#)

#target_link_libraries(sdl2_gui_test kanima mingw32 SDL2)

add_library(sdl2_gui
    sdl_gui.cpp
)

target_link_libraries(sdl2_gui
    kanima
    mingw32
    SDL2
)

target_include_directories(sdl2_gui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
