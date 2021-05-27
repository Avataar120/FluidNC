#pragma once

// The definitions herein are used only in Stepper.cpp
// They are not part of the public interface to the Stepper code.

#ifndef SEGMENT_BUFFER_SIZE
#    define SEGMENT_BUFFER_SIZE 6
#endif

// Some useful constants.
const double DT_SEGMENT              = (1.0 / (ACCELERATION_TICKS_PER_SECOND * 60.0));  // min/segment
const double REQ_MM_INCREMENT_SCALAR = 1.25;
const int    RAMP_ACCEL              = 0;
const int    RAMP_CRUISE             = 1;
const int    RAMP_DECEL              = 2;
const int    RAMP_DECEL_OVERRIDE     = 3;

struct PrepFlag {
    uint8_t recalculate : 1;
    uint8_t holdPartialBlock : 1;
    uint8_t parking : 1;
    uint8_t decelOverride : 1;
};

const timer_group_t STEP_TIMER_GROUP = TIMER_GROUP_0;
const timer_idx_t   STEP_TIMER_INDEX = TIMER_0;

// fStepperTimer should be an integer divisor of the bus speed, i.e. of fTimers
const uint32_t fStepperTimer       = 20000000;  // frequency of step pulse timer
const int      ticksPerMicrosecond = fStepperTimer / 1000000;

// Define Adaptive Multi-Axis Step-Smoothing(AMASS) levels and cutoff frequencies. The highest level
// frequency bin starts at 0Hz and ends at its cutoff frequency. The next lower level frequency bin
// starts at the next higher cutoff frequency, and so on. The cutoff frequencies for each level must
// be considered carefully against how much it over-drives the stepper ISR, the accuracy of the 16-bit
// timer, and the CPU overhead. Level 0 (no AMASS, normal operation) frequency bin starts at the
// Level 1 cutoff frequency and up to as fast as the CPU allows (over 30kHz in limited testing).
// For efficient computation, each cutoff frequency is twice the previous one.
// NOTE: AMASS cutoff frequency multiplied by ISR overdrive factor must not exceed maximum step frequency.
// NOTE: Current settings are set to overdrive the ISR to no more than 16kHz, balancing CPU overhead
// and timer accuracy.  Do not alter these settings unless you know what you are doing.

const uint32_t amassThreshold = fStepperTimer / 8000;
const int      maxAmassLevel  = 3;  // Each level increase doubles the threshold
