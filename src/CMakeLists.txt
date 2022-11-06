
add_subdirectory(math)
add_subdirectory(sound)

add_library(camera camera.cc)
add_library(events events.cc)
add_library(window window.cc)

add_executable(main main.cc)

# Q: Why "-Wl,--whole-archive" ???
#
# https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
# Binding code is run as a static constructor and static constructors
# only get run if the object file is included in the link, therefore
# when generating bindings for library files the compiler must be
# explicitly instructed to include the object file.
target_link_libraries(main
    -Wl,--whole-archive
    camera
    events
    window
)

set_target_properties(main PROPERTIES
    LINK_FLAGS "-s USE_SDL=2 -s FULL_ES2=1 -s WASM=1 --bind"
)

# add_executable(synth synth.cc)
# target_link_libraries(synth
#     -Wl,--whole-archive
#     dual_fm_osc
#     audio_frame
# )
# set_target_properties(synth PROPERTIES
#     LINK_FLAGS "-s WASM=1 --bind"
# )
