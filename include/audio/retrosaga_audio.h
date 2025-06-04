/*
 * RetroSaga Audio Subsystem Header
 * Aegis Project Phase 1 Implementation
 * 
 * Comprehensive audio pipeline with DSS compliance
 */

#ifndef RETROSAGA_AUDIO_H
#define RETROSAGA_AUDIO_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// Return codes
#define RETROSAGA_SUCCESS                    0
#define RETROSAGA_ERROR_NOT_INITIALIZED     -1
#define RETROSAGA_ERROR_ALREADY_INITIALIZED -2
#define RETROSAGA_ERROR_INVALID_PARAM       -3
#define RETROSAGA_ERROR_CRYPTO_VALIDATION   -4
#define RETROSAGA_ERROR_AUDIO_INIT          -5
#define RETROSAGA_ERROR_MIDI_INIT           -6

// Audio configuration
#define RETROSAGA_SAMPLE_RATE    44100
#define RETROSAGA_BUFFER_SIZE    1024
#define RETROSAGA_MAX_POLYPHONY  64
#define RETROSAGA_MAX_CHANNELS   16

// MIDI message types
typedef enum {
    MIDI_NOTE_OFF = 0x80,
    MIDI_NOTE_ON = 0x90,
    MIDI_POLY_PRESSURE = 0xA0,
    MIDI_CONTROL_CHANGE = 0xB0,
    MIDI_PROGRAM_CHANGE = 0xC0,
    MIDI_CHANNEL_PRESSURE = 0xD0,
    MIDI_PITCH_BEND = 0xE0,
    MIDI_SYSTEM_EXCLUSIVE = 0xF0
} midi_message_type_t;

// Audio processing modules
int retrosaga_audio_init(void);
int retrosaga_audio_update(float delta_time_ms);
void retrosaga_audio_shutdown(void);
bool retrosaga_audio_validate(void);

// Input modules
int input_audio_init(void);
int audio_entropy_init(void);
int prng_module_init(void);

// Processing modules  
int midi_processing_init(void);
int bit_scaler_init(void);
int effect_engine_init(void);

// Output modules
int waveform_generator_init(void);
int sound_output_init(void);

// MIDI processing
int process_midi_message(uint8_t status, uint8_t data1, uint8_t data2);
int scale_midi_value(uint32_t value, uint8_t src_bits, uint8_t dst_bits);

// Waveform generation
int generate_waveform(float frequency, float amplitude, float* buffer, size_t samples);
int output_audio_buffer(const float* buffer, size_t samples);

// Diagnostic interface
int retrosaga_audio_diagnose(void);

#ifdef __cplusplus
}
#endif

#endif // RETROSAGA_AUDIO_H
