/*
 * Bit Scaler Module
 * Implements MIDI 2.0 bit scaling algorithms
 * Based on M2-115-U specification
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "audio/bit_scaler.h"
#include <string.h>
#include <stdlib.h>

// Power of 2 calculation using bit shift
static uint32_t power_of_2(uint8_t exp) {
    return 1 << exp;
}

// Min-Center-Max upscaling algorithm (Section 3.3 of M2-115-U)
uint32_t scale_midi_value_min_center_max(uint32_t src_val, uint8_t src_bits, uint8_t dst_bits) {
    if (src_bits >= dst_bits) {
        // Downscaling - simple bit shift
        uint8_t scale_bits = src_bits - dst_bits;
        return src_val >> scale_bits;
    }
    
    // Upscaling
    if (src_bits == 1) {
        // Special case for 1-bit: 0 -> 0, 1 -> max value
        return (src_val == 0) ? 0 : (power_of_2(dst_bits) - 1);
    }
    
    uint8_t scale_bits = dst_bits - src_bits;
    uint32_t src_center = power_of_2(src_bits - 1);
    uint32_t bit_shifted_value = src_val << scale_bits;
    
    if (src_val <= src_center) {
        return bit_shifted_value;
    }
    
    // Expanded bit repeat scheme for values above center
    uint8_t repeat_bits = src_bits - 1;
    uint32_t repeat_mask = power_of_2(repeat_bits) - 1;
    uint32_t repeat_value = src_val & repeat_mask;
    
    if (scale_bits > repeat_bits) {
        repeat_value <<= (scale_bits - repeat_bits);
    } else {
        repeat_value >>= (repeat_bits - scale_bits);
    }
    
    while (repeat_value != 0) {
        bit_shifted_value |= repeat_value;
        repeat_value >>= repeat_bits;
    }
    
    return bit_shifted_value;
}

// Zero-extension upscaling algorithm (Section 4.3 of M2-115-U)
uint32_t scale_midi_value_zero_extension(uint32_t src_val, uint8_t src_bits, uint8_t dst_bits) {
    if (src_bits >= dst_bits) {
        // Downscaling with rounding
        uint8_t scale_bits = src_bits - dst_bits;
        uint32_t half_scale_range = 1 << (scale_bits - 1);
        uint32_t shifted = (src_val + half_scale_range) >> scale_bits;
        uint32_t max_value = (1 << dst_bits) - 1;
        
        return (shifted > max_value) ? max_value : shifted;
    }
    
    // Upscaling - simple bit shift (zero extension)
    uint8_t scale_bits = dst_bits - src_bits;
    return src_val << scale_bits;
}

// Generic scaling function with method selection
int scale_midi_value(uint32_t value, uint8_t src_bits, uint8_t dst_bits) {
    // Use Min-Center-Max scaling for most MIDI values
    return scale_midi_value_min_center_max(value, src_bits, dst_bits);
}

int bit_scaler_init(void) {
    printf("[BIT_SCALER] Bit scaler initialized with MIDI 2.0 algorithms\n");
    return RETROSAGA_SUCCESS;
}

int bit_scaler_process(void) {
    return RETROSAGA_SUCCESS;
}

void bit_scaler_shutdown(void) {
    printf("[BIT_SCALER] Bit scaler shutdown complete\n");
}

bool bit_scaler_validate(void) {
    // Test scaling algorithms with known values
    uint32_t test_7_to_16 = scale_midi_value_min_center_max(127, 7, 16);
    if (test_7_to_16 != 65535) {
        printf("[BIT_SCALER] VALIDATION FAILED: 7->16 bit scaling incorrect\n");
        return false;
    }
    
    uint32_t test_center = scale_midi_value_min_center_max(64, 7, 16);
    if (test_center != 32768) {
        printf("[BIT_SCALER] VALIDATION FAILED: Center value scaling incorrect\n");
        return false;
    }
    
    printf("[BIT_SCALER] Bit scaler validation passed\n");
    return true;
}
