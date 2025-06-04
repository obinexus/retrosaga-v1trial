/*
 * Audio_entropy Module Header
 * Aegis Project Phase 1 Implementation
 */

#ifndef AUDIO_ENTROPY_H
#define AUDIO_ENTROPY_H

#include <stdint.h>
#include <stdbool.h>
#include "retrosaga_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-specific functions
int audio_entropy_init(void);
int audio_entropy_process(void);
void audio_entropy_shutdown(void);
bool audio_entropy_validate(void);

#ifdef __cplusplus
}
#endif

#endif // AUDIO_ENTROPY_H
