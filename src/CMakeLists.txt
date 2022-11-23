
add_subdirectory(math)
add_subdirectory(sound)

# add_subdirectory(opengl_test)

# Q: Why "-Wl,--whole-archive" ???
#
# https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
# Binding code is run as a static constructor and static constructors
# only get run if the object file is included in the link, therefore
# when generating bindings for library files the compiler must be
# explicitly instructed to include the object file.
add_executable(synth synth.cc)

target_link_libraries(synth
-Wl,--whole-archive
lib-dual-fm-osc
lib-audio-frame
lib-math-tables)

set_target_properties(synth PROPERTIES
    LINK_FLAGS "-s WASM=1 \
                -s SINGLE_FILE=1 \
                -s MODULARIZE=1 \
                -s EXPORT_ES6=1 \
                -s ENVIRONMENT=shell \
                --bind")
