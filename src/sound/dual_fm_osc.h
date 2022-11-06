/**
 * @copyright Dan Green Copyright (c) 2022.
 * Modified by Matt Shilling.
 */

#pragma once

#include "math/math.h"
#include "math/math_tables.h"
#include "sound/audio_stream_config.h"
#include "sound/oscillators.h"

class DualFmOsc {
  RampOscillator<48000> ramp{1};
  TriangleOscillator<48000> modulator{40};
  SineOscillator<48000> carrier{250};

public:
  AudioStreamConfig::AudioOutBuffer &
  process(AudioStreamConfig::AudioInBuffer &in_buffer,
          AudioStreamConfig::AudioOutBuffer &out_buffer);
};
