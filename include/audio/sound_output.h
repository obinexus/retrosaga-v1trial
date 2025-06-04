/*
 * Sound_output Module Header
 * Aegis Project Phase 1 Implementation
 */

#ifndef SOUND_OUTPUT_H
#define SOUND_OUTPUT_H

#include <stdint.h>
#include <stdbool.h>
#include "retrosaga_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-specific functions
int sound_output_init(void);
int sound_output_process(void);
void sound_output_shutdown(void);
bool sound_output_validate(void);

#ifdef __cplusplus
}
#endif

#endif // SOUND_OUTPUT_H
