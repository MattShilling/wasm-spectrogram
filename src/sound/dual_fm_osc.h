/**
 * @copyright Dan Green Copyright (c) 2022.
 * Modified by Matt Shilling.
 */

#pragma once

#include <map>
#include <string>

#include "math/math.h"
#include "math/math_tables.h"
#include "sound/audio_stream_config.h"
#include "sound/oscillators.h"

const LookupTableMap kOscillatorTable{{"sin", &sinTable},
                                      {"triangle", &triangleTable}};

class DualFmOsc {
  RampOscillator<48000> ramp{1};
  // TriangleOscillator<48000> modulator{40};
  Oscillator<48000> modulator =
      Oscillator<48000>(40, kOscillatorTable, "triangle");

  // SineOscillator<48000> carrier{250};
  Oscillator<48000> carrier = Oscillator<48000>(250, kOscillatorTable, "sin");

public:
  AudioStreamConfig::AudioOutBuffer &
  process(AudioStreamConfig::AudioInBuffer &in_buffer,
          AudioStreamConfig::AudioOutBuffer &out_buffer);
};
