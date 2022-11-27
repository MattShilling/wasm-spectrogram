/**
 * @copyright Dan Green Copyright (c) 2022.
 * Modified by Matt Shilling.
 */

#include <array>
#include <cstdint>
#include <type_traits>

#include "math/math.h"
#include "sound/audio_frame.h"

template <typename SampleType, int UsedBits, int NumChannels>
constexpr float
AudioFrame<SampleType, UsedBits, NumChannels>::scaleInput(SampleType value) {
  return sign_extend(value) / kOutScaling;
}

template <typename SampleType, int UsedBits, int NumChannels>
inline constexpr SampleType
AudioFrame<SampleType, UsedBits, NumChannels>::scaleOutput(const float value) {
  if (std::is_signed<SampleType>::value) {
    const float constrained_value =
        MathTools::constrain(value, -1.f, (kOutScaling - 1.f) / kOutScaling);
    return static_cast<SampleType>(constrained_value * kOutScaling);
  } else {
    const float constrained_value =
        MathTools::constrain(value * 0.5f + 0.5f, 0.f, 1.0f);
    return static_cast<SampleType>(constrained_value) *
           (MathTools::ipow(2, UsedBits) - 1);
  }
}

template <typename SampleType, int UsedBits, int NumChannels>
constexpr SampleType AudioFrame<SampleType, UsedBits, NumChannels>::sign_extend(
    const SampleType value) noexcept {
  if ((sizeof(value) * 8u) == UsedBits) {
    // No sign extension needed.
    return value;
  } else {
    struct {
      signed value : UsedBits;
    } s;
    return s.value = value;
  }
}

template class AudioFrame<uint32_t, 24, 2>;

#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(AudioFrame) {
  emscripten::function("scaleInput", &AudioFrame<uint32_t, 24, 2>::scaleInput);

  emscripten::value_array<std::array<uint32_t, 2>>("array_uint32_t_2")
      .element(emscripten::index<0>())
      .element(emscripten::index<1>());

  emscripten::value_object<AudioFrame<uint32_t, 24, 2>>(
      "AudioFrame<uint32_t, 24, 2>")
      .field("chan", &AudioFrame<uint32_t, 24, 2>::chan);

  /**
   * @brief Array of 32 AudioFrames. This is the number of audio blocks defined
   * in AudioStreamConfig.
   */
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
}
