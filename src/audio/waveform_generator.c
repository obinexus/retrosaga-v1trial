/*
 * Waveform Generator Module
 * Generates basic waveforms for synthesis
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "audio/waveform_generator.h"
#include <string.h>
#include <stdlib.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    bool initialized;
    float sample_rate;
    uint32_t waveforms_generated;
} waveform_generator_state_t;

static waveform_generator_state_t g_waveform_state = {0};

int waveform_generator_init(void) {
    if (g_waveform_state.initialized) {
        return RETROSAGA_ERROR_ALREADY_INITIALIZED;
    }
    
    printf("[WAVEFORM_GENERATOR] Initializing waveform generator...\n");
    
    g_waveform_state.sample_rate = RETROSAGA_SAMPLE_RATE;
    g_waveform_state.waveforms_generated = 0;
    g_waveform_state.initialized = true;
    
    printf("[WAVEFORM_GENERATOR] Waveform generator initialized at %d Hz\n", 
           RETROSAGA_SAMPLE_RATE);
    return RETROSAGA_SUCCESS;
}

// Generate sine wave
static void generate_sine_wave(float frequency, float amplitude, float* buffer, size_t samples) {
    for (size_t i = 0; i < samples; i++) {
        float t = (float)i / g_waveform_state.sample_rate;
        buffer[i] = amplitude * sinf(2.0f * M_PI * frequency * t);
    }
}

// Generate sawtooth wave
static void generate_sawtooth_wave(float frequency, float amplitude, float* buffer, size_t samples) {
    for (size_t i = 0; i < samples; i++) {
        float t = (float)i / g_waveform_state.sample_rate;
        float phase = fmodf(frequency * t, 1.0f);
        buffer[i] = amplitude * (2.0f * phase - 1.0f);
    }
}

// Generate square wave
static void generate_square_wave(float frequency, float amplitude, float* buffer, size_t samples) {
    for (size_t i = 0; i < samples; i++) {
        float t = (float)i / g_waveform_state.sample_rate;
        float phase = fmodf(frequency * t, 1.0f);
        buffer[i] = amplitude * (phase < 0.5f ? 1.0f : -1.0f);
    }
}

// Generate triangle wave
static void generate_triangle_wave(float frequency, float amplitude, float* buffer, size_t samples) {
    for (size_t i = 0; i < samples; i++) {
        float t = (float)i / g_waveform_state.sample_rate;
        float phase = fmodf(frequency * t, 1.0f);
        buffer[i] = amplitude * (phase < 0.5f ? 4.0f * phase - 1.0f : 3.0f - 4.0f * phase);
    }
}

int generate_waveform(float frequency, float amplitude, float* buffer, size_t samples) {
    if (!g_waveform_state.initialized || !buffer) {
        return RETROSAGA_ERROR_INVALID_PARAM;
    }
    
    // Default to sine wave for now
    generate_sine_wave(frequency, amplitude, buffer, samples);
    
    g_waveform_state.waveforms_generated++;
    return RETROSAGA_SUCCESS;
}

int waveform_generator_process(void) {
    if (!g_waveform_state.initialized) {
        return RETROSAGA_ERROR_NOT_INITIALIZED;
    }
    
    return RETROSAGA_SUCCESS;
}

void waveform_generator_shutdown(void) {
    if (!g_waveform_state.initialized) {
        return;
    }
    
    printf("[WAVEFORM_GENERATOR] Shutting down waveform generator...\n");
    printf("[WAVEFORM_GENERATOR] Waveforms generated: %d\n", 
           g_waveform_state.waveforms_generated);
    
    memset(&g_waveform_state, 0, sizeof(g_waveform_state));
    printf("[WAVEFORM_GENERATOR] Waveform generator shutdown complete\n");
}

bool waveform_generator_validate(void) {
    if (!g_waveform_state.initialized) {
        printf("[WAVEFORM_GENERATOR] VALIDATION FAILED: Not initialized\n");
        return false;
    }
    
    // Test waveform generation
    float test_buffer[64];
    if (generate_waveform(440.0f, 0.5f, test_buffer, 64) != RETROSAGA_SUCCESS) {
        printf("[WAVEFORM_GENERATOR] VALIDATION FAILED: Cannot generate test waveform\n");
        return false;
    }
    
    printf("[WAVEFORM_GENERATOR] Waveform generator validation passed\n");
    return true;
}
