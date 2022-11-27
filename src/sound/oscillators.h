/**
 * @copyright Dan Green Copyright (c) 2022.
 * Modified by Matt Shilling.
 */

#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "math/math_tables.h"

// template<int UpdateRateHz>
// struct ProtoOscillator {
// 	ProtoOscillator(uint32_t freq);
// 	ProtoOscillator(float freq);

// 	void set_frequency(uint32_t freq);
// 	void set_frequency(float freq);
// 	void set_phase(float phase);

// 	void update();
// 	uint32_t val();
// 	uint32_t get_int() {return val();}
// 	float get_float();
// 	float get_bipolar_float();

// 	uint32_t process() {update(); return val();}
// 	uint32_t process_as_float() {update(); return get_float();}
// 	uint32_t process_as_bipolar_float() {update(); return
// get_bipolar_float();}
// };

// actual freq = update_rate(Hz) * increment(#) / 2^32
// increment = 2^32 * freq / update_rate

template <int UpdateRateHz> struct PhaseAccum {
  PhaseAccum(uint32_t freq) : increment_(freq * freq_units_) {}
  PhaseAccum() : increment_(0) {}

  void update() { phase_ += increment_; }

  uint32_t val() { return phase_; }

  uint32_t process() {
    update();
    return val();
  }

  float process_float() {
    return static_cast<float>(process()) * to_float_convert_;
  }

  void set_frequency(uint32_t freq) { increment_ = freq * freq_units_; }

  void set_frequency(float freq) { increment_ = freq * freq_units_float_; }

  void set_phase(uint32_t phase) { phase_ = phase; }

private:
  static constexpr uint32_t max_ = 0xFFFFFFFF;
  static constexpr float to_float_convert_ =
      1.0f / 4294967295.f; // stores as 0x2f800000
  static constexpr uint32_t freq_units_ = max_ / UpdateRateHz;
  static constexpr float freq_units_float_ =
      static_cast<float>(max_) / static_cast<float>(UpdateRateHz);
  uint32_t phase_ = 0;
  uint32_t increment_;
};

/**
 * @brief A ramp oscillator is just a phase accumulator.
 *
 * @tparam UpdateRateHz
 */
template <int UpdateRateHz> using RampOscillator = PhaseAccum<UpdateRateHz>;

using LookupTableMap = std::map<std::string, const LookupTable *>;

template <int UpdateRateHz> struct Oscillator {
  Oscillator(uint32_t freq, LookupTableMap table_ptrs_map,
             std::string default_table)
      : phaseacc_{freq}, table_ptrs_map_(table_ptrs_map),
        current_table_(table_ptrs_map_.at(default_table)) {}
  Oscillator() = default;

  uint32_t value() { return process(); }

  float normalized_value() { return process_normalized(); }

  void set_frequency(uint32_t freq) { phaseacc_.set_frequency(freq); }

  void set_frequency(float freq) { phaseacc_.set_frequency(freq); }

  void set_phase(uint32_t phase) { phaseacc_.set_phase(phase); }

  void set_table(std::string table_name) {
    if (table_ptrs_map_.count(table_name) != 0) {
      current_table_ = table_ptrs_map_.at(table_name);
    }
  }

private:
  // Returns 0 .. almost 0xFFFFFFFF
  uint32_t process() {
    return static_cast<uint32_t>(process_table_normalized() * to_u32_convert_);
  }

  float process_normalized() { return process_table_normalized(); }

  // Returns raw table value, not normalized to 0 .. +1.
  float table_at_phase() {
    return current_table_->interp_wrap(phaseacc_.process_float());
  }

  // Returns 0 .. +1.
  float process_table_normalized() {
    return (table_at_phase() - current_table_->min) /
           (current_table_->max - current_table_->min);
  }

private:
  PhaseAccum<UpdateRateHz> phaseacc_;
  const LookupTableMap table_ptrs_map_;
  const LookupTable *current_table_ = nullptr;
  size_t table_idx_ = 0;
  static constexpr uint32_t max_ = 0xFFFFFFFF;
  static constexpr float to_u32_convert_ =
      4294967040.f; // largest value less than 4294967296.f
};
