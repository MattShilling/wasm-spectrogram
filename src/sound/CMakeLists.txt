add_library(lib-audio-frame audio_frame.cc)

add_library(lib-dual-fm-osc dual_fm_osc.cc)
target_link_libraries(lib-dual-fm-osc lib-math-tables)
