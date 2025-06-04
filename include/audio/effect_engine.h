/*
 * Effect_engine Module Header
 * Aegis Project Phase 1 Implementation
 */

#ifndef EFFECT_ENGINE_H
#define EFFECT_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include "retrosaga_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-specific functions
int effect_engine_init(void);
int effect_engine_process(void);
void effect_engine_shutdown(void);
bool effect_engine_validate(void);

#ifdef __cplusplus
}
#endif

#endif // EFFECT_ENGINE_H
