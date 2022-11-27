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

    float freq_mult_2 = exp5Table.interp(modulator.normalized_value());
    carrier.set_frequency(20 * freq_mult_2);

    out.chan[0] = modulator.value() >> 8;
    out.chan[1] = carrier.value() >> 8;
  }

  return out_buffer;
}

#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(DualFmOsc) {
  emscripten::class_<DualFmOsc>("DualFmOsc")
      .constructor<>()
      .function("process", &DualFmOsc::process);
}
