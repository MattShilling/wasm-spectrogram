/**
 * @copyright Dan Green Copyright (c) 2022.
 * Modified by Matt Shilling.
 */

#pragma once

#include "interp_array.h"

using LookupTable = InterpArray<float, 2048>;

extern const LookupTable sinTable;
extern const LookupTable tanTable;

extern const LookupTable triangleTable;

/**
 * @brief e^x on the y-axis range [0, 32].
 * x-axis is divided into log(32) / 2047 slices, starting at 0.
 * 32 comes from 2^5, hence the name exp 5 table.
 */
extern const LookupTable exp5Table;

/**
 * @brief e^x on the y-axis range [0, 1024].
 * x-axis is divided into log(1024) / 2047 slices.
 * 1024 comes from 2^10, hence the name exp 10 table.
 */
extern const LookupTable exp10Table;

extern const LookupTable pow9Table;
extern const LookupTable logTable;