/**
 * @copyright Dan Green Copyright (c) 2022.
 * Modified by Matt Shilling.
 */

#pragma once

#include <array>
#include <cstdint>
#include <type_traits>

#include "math/math.h"

template <typename SampleType, int UsedBits = sizeof(SampleType) * 8,
          int NumChannels = 2>
class AudioFrame {
  static_assert((sizeof(SampleType) * 8u) >= UsedBits,
                "SampleType is smaller than the specified width");

public:
  static constexpr unsigned kSampleTypeBits = sizeof(SampleType) * 8;
  static constexpr unsigned kMaxValue = MathTools::ipow(2, UsedBits - 1);
  static constexpr float kOutScaling = static_cast<float>(kMaxValue);

  static inline constexpr float scaleInput(SampleType val);

  static inline constexpr SampleType scaleOutput(const float val);

  static inline constexpr SampleType sign_extend(const SampleType &v) noexcept;

  SampleType getChannelOne() const { return chan[0]; }
  void setChannelOne(SampleType v) { chan[0] = v; }

  SampleType getChannelTwo() const { return chan[1]; }
  void setChannelTwo(SampleType v) { chan[1] = v; }

  std::array<SampleType, NumChannels> chan;
};