add_library(test test.cc)

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
    test
)

set_target_properties(main PROPERTIES
    LINK_FLAGS "-s WASM=1 -s --bind"
)