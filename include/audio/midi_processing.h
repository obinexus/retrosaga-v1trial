/*
 * Midi_processing Module Header
 * Aegis Project Phase 1 Implementation
 */

#ifndef MIDI_PROCESSING_H
#define MIDI_PROCESSING_H

#include <stdint.h>
#include <stdbool.h>
#include "retrosaga_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-specific functions
int midi_processing_init(void);
int midi_processing_process(void);
void midi_processing_shutdown(void);
bool midi_processing_validate(void);

#ifdef __cplusplus
}
#endif

#endif // MIDI_PROCESSING_H
