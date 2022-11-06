#include <array>
#include <cstdint>
#include <type_traits>

#include "math/math.h"
#include "sound/audio_frame.h"

template <typename SampleType, int UsedBits, int NumChannels>
constexpr float
AudioFrame<SampleType, UsedBits, NumChannels>::scaleInput(SampleType val) {
  return sign_extend(val) / kOutScaling;
}

template <typename SampleType, int UsedBits, int NumChannels>
inline constexpr SampleType
AudioFrame<SampleType, UsedBits, NumChannels>::scaleOutput(const float val) {
  if (std::is_signed<SampleType>::value) {
    const float v =
        MathTools::constrain(val, -1.f, (kOutScaling - 1.f) / kOutScaling);
    return static_cast<SampleType>(v * kOutScaling);
  } else {
    const float v = MathTools::constrain(val * 0.5f + 0.5f, 0.f, 1.0f);
    return static_cast<SampleType>(v) * (MathTools::ipow(2, UsedBits) - 1);
  }
}

template <typename SampleType, int UsedBits, int NumChannels>
inline constexpr SampleType
AudioFrame<SampleType, UsedBits, NumChannels>::sign_extend(
    const SampleType &v) noexcept {
  if ((sizeof(SampleType) * 8u) == UsedBits) {
    return v;
  } else {
    using S = struct { signed Val : UsedBits; };
    return reinterpret_cast<const S *>(&v)->Val;
  }
}

template class AudioFrame<uint32_t, sizeof(uint32_t) * 8, 2>;

#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(AudioFrame) {
  emscripten::class_<AudioFrame<uint32_t, sizeof(uint32_t) * 8, 2>>(
      "AudioFrame")
      .constructor<>()
      .property("chan_one",
                &AudioFrame<uint32_t, sizeof(uint32_t) * 8, 2>::getChannelOne,
                &AudioFrame<uint32_t, sizeof(uint32_t) * 8, 2>::setChannelOne)
      .property("chan_two",
                &AudioFrame<uint32_t, sizeof(uint32_t) * 8, 2>::getChannelTwo,
                &AudioFrame<uint32_t, sizeof(uint32_t) * 8, 2>::setChannelTwo)
      .class_function(
          "scaleInput",
          &AudioFrame<uint32_t, sizeof(uint32_t) * 8, 2>::scaleInput);
}