/*
 * Input_audio Module Header
 * Aegis Project Phase 1 Implementation
 */

#ifndef INPUT_AUDIO_H
#define INPUT_AUDIO_H

#include <stdint.h>
#include <stdbool.h>
#include "retrosaga_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-specific functions
int input_audio_init(void);
int input_audio_process(void);
void input_audio_shutdown(void);
bool input_audio_validate(void);

#ifdef __cplusplus
}
#endif

#endif // INPUT_AUDIO_H
