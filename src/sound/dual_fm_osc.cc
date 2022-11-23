/**
 * @copyright Dan Green Copyright (c) 2022.
 * Modified by Matt Shilling.
 */

#include "sound/dual_fm_osc.h"
#include "sound/audio_stream_config.h"

AudioStreamConfig::AudioOutBuffer &
DualFmOsc::process(AudioStreamConfig::AudioInBuffer &in_buffer,
                   AudioStreamConfig::AudioOutBuffer &out_buffer) {
  for (auto &out : out_buffer) {
    float freq_mult_1 = exp5Table.interp(ramp.process_float());
    modulator.set_frequency(40 * freq_mult_1);

    float freq_mult_2 = exp5Table.interp(modulator.process_float());
    carrier.set_frequency(20 * freq_mult_2);
    carrier.process();

    out.chan[0] = modulator.val() >> 8;
    out.chan[1] = carrier.val() >> 8;
  }

  return out_buffer;
}

#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(DualFmOsc) {
  emscripten::value_array<std::array<AudioFrame<uint32_t, 24, 2>, 32>>(
      "array_AudioFrame<uint32_t, 24, 2>_32")
      .element(emscripten::index<0>())
      .element(emscripten::index<1>())
      .element(emscripten::index<2>())
      .element(emscripten::index<3>())
      .element(emscripten::index<4>())
      .element(emscripten::index<5>())
      .element(emscripten::index<6>())
      .element(emscripten::index<7>())
      .element(emscripten::index<8>())
      .element(emscripten::index<9>())
      .element(emscripten::index<10>())
      .element(emscripten::index<11>())
      .element(emscripten::index<12>())
      .element(emscripten::index<13>())
      .element(emscripten::index<14>())
      .element(emscripten::index<15>())
      .element(emscripten::index<16>())
      .element(emscripten::index<17>())
      .element(emscripten::index<18>())
      .element(emscripten::index<19>())
      .element(emscripten::index<20>())
      .element(emscripten::index<21>())
      .element(emscripten::index<22>())
      .element(emscripten::index<23>())
      .element(emscripten::index<24>())
      .element(emscripten::index<25>())
      .element(emscripten::index<26>())
      .element(emscripten::index<27>())
      .element(emscripten::index<28>())
      .element(emscripten::index<29>())
      .element(emscripten::index<30>())
      .element(emscripten::index<31>());

  emscripten::class_<DualFmOsc>("DualFmOsc")
      .constructor<>()
      .function("process", &DualFmOsc::process);
}
