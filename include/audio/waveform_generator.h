/*
 * Waveform_generator Module Header
 * Aegis Project Phase 1 Implementation
 */

#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "retrosaga_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-specific functions
int waveform_generator_init(void);
int waveform_generator_process(void);
void waveform_generator_shutdown(void);
bool waveform_generator_validate(void);

#ifdef __cplusplus
}
#endif

#endif // WAVEFORM_GENERATOR_H
