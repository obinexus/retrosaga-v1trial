/*
 * Prng_module Module Header
 * Aegis Project Phase 1 Implementation
 */

#ifndef PRNG_MODULE_H
#define PRNG_MODULE_H

#include <stdint.h>
#include <stdbool.h>
#include "retrosaga_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-specific functions
int prng_module_init(void);
int prng_module_process(void);
void prng_module_shutdown(void);
bool prng_module_validate(void);

#ifdef __cplusplus
}
#endif

#endif // PRNG_MODULE_H
