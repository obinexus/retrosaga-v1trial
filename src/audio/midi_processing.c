/*
 * MIDI Processing Module
 * Implements MIDI 2.0 bit scaling and resolution handling
 * Based on M2-115-U specification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "audio/midi_processing.h"
#include "audio/bit_scaler.h"

typedef struct {
    bool initialized;
    uint32_t messages_processed;
    uint8_t active_channels[16];
    float channel_volumes[16];
} midi_processor_state_t;

static midi_processor_state_t g_midi_state = {0};

int midi_processing_init(void) {
    if (g_midi_state.initialized) {
        return RETROSAGA_ERROR_ALREADY_INITIALIZED;
    }
    
    printf("[MIDI_PROCESSING] Initializing MIDI processor with bit scaling support...\n");
    
    // Initialize channel states
    for (int i = 0; i < 16; i++) {
        g_midi_state.active_channels[i] = 0;
        g_midi_state.channel_volumes[i] = 1.0f;
    }
    
    g_midi_state.messages_processed = 0;
    g_midi_state.initialized = true;
    
    printf("[MIDI_PROCESSING] MIDI processor initialized successfully\n");
    return RETROSAGA_SUCCESS;
}

// Process MIDI message with proper bit scaling
int process_midi_message(uint8_t status, uint8_t data1, uint8_t data2) {
    if (!g_midi_state.initialized) {
        return RETROSAGA_ERROR_NOT_INITIALIZED;
    }
    
    uint8_t channel = status & 0x0F;
    uint8_t message_type = status & 0xF0;
    
    switch (message_type) {
        case MIDI_NOTE_ON:
            if (data2 > 0) { // Velocity > 0 means note on
                printf("[MIDI_PROCESSING] Note ON: Ch %d, Note %d, Vel %d\n", 
                       channel + 1, data1, data2);
                g_midi_state.active_channels[channel]++;
                
                // Scale velocity from 7-bit to 16-bit using Min-Center-Max scaling
                uint32_t scaled_velocity = scale_midi_value_min_center_max(data2, 7, 16);
                printf("[MIDI_PROCESSING] Scaled velocity: %d -> %d\n", data2, scaled_velocity);
            } else {
                // Velocity 0 means note off
                printf("[MIDI_PROCESSING] Note OFF: Ch %d, Note %d\n", channel + 1, data1);
                if (g_midi_state.active_channels[channel] > 0) {
                    g_midi_state.active_channels[channel]--;
                }
            }
            break;
            
        case MIDI_NOTE_OFF:
            printf("[MIDI_PROCESSING] Note OFF: Ch %d, Note %d, Vel %d\n", 
                   channel + 1, data1, data2);
            if (g_midi_state.active_channels[channel] > 0) {
                g_midi_state.active_channels[channel]--;
            }
            break;
            
        case MIDI_CONTROL_CHANGE:
            printf("[MIDI_PROCESSING] Control Change: Ch %d, CC %d, Val %d\n", 
                   channel + 1, data1, data2);
                   
            // Handle volume control (CC 7)
            if (data1 == 7) {
                g_midi_state.channel_volumes[channel] = (float)data2 / 127.0f;
                printf("[MIDI_PROCESSING] Channel %d volume: %.2f\n", 
                       channel + 1, g_midi_state.channel_volumes[channel]);
            }
            break;
            
        case MIDI_PITCH_BEND:
            {
                // Combine 7-bit values into 14-bit pitch bend
                uint16_t pitch_bend = (data2 << 7) | data1;
                printf("[MIDI_PROCESSING] Pitch Bend: Ch %d, Value %d\n", 
                       channel + 1, pitch_bend);
            }
            break;
            
        default:
            printf("[MIDI_PROCESSING] Unsupported message type: 0x%02X\n", message_type);
            break;
    }
    
    g_midi_state.messages_processed++;
    return RETROSAGA_SUCCESS;
}

int midi_processing_process(void) {
    if (!g_midi_state.initialized) {
        return RETROSAGA_ERROR_NOT_INITIALIZED;
    }
    
    // Process pending MIDI messages from queue
    // This would typically read from a MIDI input buffer
    
    return RETROSAGA_SUCCESS;
}

void midi_processing_shutdown(void) {
    if (!g_midi_state.initialized) {
        return;
    }
    
    printf("[MIDI_PROCESSING] Shutting down MIDI processor...\n");
    printf("[MIDI_PROCESSING] Total messages processed: %d\n", g_midi_state.messages_processed);
    
    memset(&g_midi_state, 0, sizeof(g_midi_state));
    printf("[MIDI_PROCESSING] MIDI processor shutdown complete\n");
}

bool midi_processing_validate(void) {
    if (!g_midi_state.initialized) {
        printf("[MIDI_PROCESSING] VALIDATION FAILED: Not initialized\n");
        return false;
    }
    
    printf("[MIDI_PROCESSING] MIDI processor validation passed\n");
    return true;
}
