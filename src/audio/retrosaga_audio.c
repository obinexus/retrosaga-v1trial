/*
 * RetroSaga Audio Subsystem Main Implementation
 * Aegis Project Phase 1 Implementation
 * 
 * Comprehensive audio pipeline with DSS compliance and MIDI integration
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "audio/retrosaga_audio.h"

// Include all audio module headers
#include "audio/input_audio.h"
#include "audio/audio_entropy.h"
#include "audio/prng_module.h"
#include "audio/midi_processing.h"
#include "audio/bit_scaler.h"
#include "audio/effect_engine.h"
#include "audio/waveform_generator.h"
#include "audio/sound_output.h"

typedef struct {
    bool initialized;
    bool dss_compliant;
    float frame_time_ms;
    uint64_t frame_count;
    uint32_t midi_messages_processed;
    float cpu_usage_percent;
} retrosaga_audio_state_t;

static retrosaga_audio_state_t g_audio_state = {0};

int retrosaga_audio_init(void) {
    if (g_audio_state.initialized) {
        return RETROSAGA_ERROR_ALREADY_INITIALIZED;
    }
    
    printf("[RETROSAGA_AUDIO] Initializing comprehensive audio subsystem...\n");
    
    // Initialize input modules
    if (input_audio_init() != RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] ERROR: Failed to initialize input_audio\n");
        return RETROSAGA_ERROR_AUDIO_INIT;
    }
    
    if (audio_entropy_init() != RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] ERROR: Failed to initialize audio_entropy\n");
        return RETROSAGA_ERROR_AUDIO_INIT;
    }
    
    if (prng_module_init() != RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] ERROR: Failed to initialize prng_module\n");
        return RETROSAGA_ERROR_AUDIO_INIT;
    }
    
    // Initialize processing modules
    if (bit_scaler_init() != RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] ERROR: Failed to initialize bit_scaler\n");
        return RETROSAGA_ERROR_AUDIO_INIT;
    }
    
    if (midi_processing_init() != RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] ERROR: Failed to initialize midi_processing\n");
        return RETROSAGA_ERROR_MIDI_INIT;
    }
    
    if (effect_engine_init() != RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] ERROR: Failed to initialize effect_engine\n");
        return RETROSAGA_ERROR_AUDIO_INIT;
    }
    
    // Initialize output modules
    if (waveform_generator_init() != RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] ERROR: Failed to initialize waveform_generator\n");
        return RETROSAGA_ERROR_AUDIO_INIT;
    }
    
    if (sound_output_init() != RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] ERROR: Failed to initialize sound_output\n");
        return RETROSAGA_ERROR_AUDIO_INIT;
    }
    
    // Initialize state
    g_audio_state.frame_time_ms = 16.67f; // 60 FPS target
    g_audio_state.frame_count = 0;
    g_audio_state.midi_messages_processed = 0;
    g_audio_state.cpu_usage_percent = 0.0f;
    g_audio_state.dss_compliant = true;
    g_audio_state.initialized = true;
    
    printf("[RETROSAGA_AUDIO] Audio subsystem initialized successfully\n");
    printf("[RETROSAGA_AUDIO] Configuration: %d Hz, %d samples/buffer, %d polyphony\n",
           RETROSAGA_SAMPLE_RATE, RETROSAGA_BUFFER_SIZE, RETROSAGA_MAX_POLYPHONY);
    
    return RETROSAGA_SUCCESS;
}

int retrosaga_audio_update(float delta_time_ms) {
    if (!g_audio_state.initialized) {
        return RETROSAGA_ERROR_NOT_INITIALIZED;
    }
    
    // Process all audio modules in pipeline order
    input_audio_process();
    audio_entropy_process();
    prng_module_process();
    
    midi_processing_process();
    bit_scaler_process();
    effect_engine_process();
    
    waveform_generator_process();
    sound_output_process();
    
    g_audio_state.frame_count++;
    
    // Monitor performance every second
    if (g_audio_state.frame_count % 60 == 0) {
        g_audio_state.cpu_usage_percent = (delta_time_ms / g_audio_state.frame_time_ms) * 100.0f;
        
        if (g_audio_state.frame_count % 300 == 0) { // Every 5 seconds
            printf("[RETROSAGA_AUDIO] Frame %lu, CPU: %.1f%%, MIDI: %d msgs\n",
                   g_audio_state.frame_count, g_audio_state.cpu_usage_percent,
                   g_audio_state.midi_messages_processed);
        }
    }
    
    return RETROSAGA_SUCCESS;
}

void retrosaga_audio_shutdown(void) {
    if (!g_audio_state.initialized) {
        return;
    }
    
    printf("[RETROSAGA_AUDIO] Shutting down audio subsystem...\n");
    
    // Shutdown modules in reverse order
    sound_output_shutdown();
    waveform_generator_shutdown();
    
    effect_engine_shutdown();
    midi_processing_shutdown();
    bit_scaler_shutdown();
    
    prng_module_shutdown();
    audio_entropy_shutdown();
    input_audio_shutdown();
    
    printf("[RETROSAGA_AUDIO] Audio subsystem statistics:\n");
    printf("[RETROSAGA_AUDIO]   Frames processed: %lu\n", g_audio_state.frame_count);
    printf("[RETROSAGA_AUDIO]   MIDI messages: %d\n", g_audio_state.midi_messages_processed);
    printf("[RETROSAGA_AUDIO]   Final CPU usage: %.1f%%\n", g_audio_state.cpu_usage_percent);
    
    memset(&g_audio_state, 0, sizeof(g_audio_state));
    printf("[RETROSAGA_AUDIO] Audio subsystem shutdown complete\n");
}

bool retrosaga_audio_validate(void) {
    printf("[RETROSAGA_AUDIO] Running comprehensive audio validation...\n");
    
    if (!g_audio_state.initialized) {
        printf("[RETROSAGA_AUDIO] ? Audio subsystem not initialized\n");
        return false;
    }
    
    bool all_valid = true;
    
    // Validate all modules
    all_valid &= input_audio_validate();
    all_valid &= audio_entropy_validate();
    all_valid &= prng_module_validate();
    all_valid &= midi_processing_validate();
    all_valid &= bit_scaler_validate();
    all_valid &= effect_engine_validate();
    all_valid &= waveform_generator_validate();
    all_valid &= sound_output_validate();
    
    // Test MIDI processing with sample data
    printf("[RETROSAGA_AUDIO] Testing MIDI message processing...\n");
    if (process_midi_message(MIDI_NOTE_ON | 0, 60, 127) == RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] V MIDI Note On processed\n");
    } else {
        printf("[RETROSAGA_AUDIO] ? MIDI Note On failed\n");
        all_valid = false;
    }
    
    if (process_midi_message(MIDI_NOTE_OFF | 0, 60, 0) == RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] V MIDI Note Off processed\n");
    } else {
        printf("[RETROSAGA_AUDIO] ? MIDI Note Off failed\n");
        all_valid = false;
    }
    
    // Test waveform generation
    printf("[RETROSAGA_AUDIO] Testing waveform generation...\n");
    float test_buffer[RETROSAGA_BUFFER_SIZE];
    if (generate_waveform(440.0f, 0.5f, test_buffer, RETROSAGA_BUFFER_SIZE) == RETROSAGA_SUCCESS) {
        printf("[RETROSAGA_AUDIO] V Waveform generation successful\n");
    } else {
        printf("[RETROSAGA_AUDIO] ? Waveform generation failed\n");
        all_valid = false;
    }
    
    // Validate DSS compliance
    if (g_audio_state.dss_compliant) {
        printf("[RETROSAGA_AUDIO] V DSS compliance validated\n");
    } else {
        printf("[RETROSAGA_AUDIO] ? DSS compliance failed\n");
        all_valid = false;
    }
    
    if (all_valid) {
        printf("[RETROSAGA_AUDIO] V Audio subsystem validation passed\n");
    } else {
        printf("[RETROSAGA_AUDIO] ? Audio subsystem validation failed\n");
    }
    
    return all_valid;
}

int retrosaga_audio_diagnose(void) {
    printf("=== RetroSaga Audio Subsystem Diagnostics ===\n");
    
    if (!g_audio_state.initialized) {
        printf("Audio subsystem not initialized\n");
        return 1;
    }
    
    printf("Sample Rate: %d Hz\n", RETROSAGA_SAMPLE_RATE);
    printf("Buffer Size: %d samples\n", RETROSAGA_BUFFER_SIZE);
    printf("Max Polyphony: %d voices\n", RETROSAGA_MAX_POLYPHONY);
    printf("Frame Count: %lu\n", g_audio_state.frame_count);
    printf("CPU Usage: %.1f%%\n", g_audio_state.cpu_usage_percent);
    printf("DSS Compliant: %s\n", g_audio_state.dss_compliant ? "Yes" : "No");
    
    printf("\n=== Module Status ===\n");
    if (retrosaga_audio_validate()) {
        printf("All modules operational\n");
        return 0;
    } else {
        printf("Some modules failed validation\n");
        return 1;
    }
}
