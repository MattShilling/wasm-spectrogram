add_library(dual_fm_osc dual_fm_osc.cc)
add_library(audio_frame audio_frame.cc)

target_link_libraries(dual_fm_osc math_tables)
