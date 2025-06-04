/*
 * Bit_scaler Module Header
 * Aegis Project Phase 1 Implementation
 */

#ifndef BIT_SCALER_H
#define BIT_SCALER_H

#include <stdint.h>
#include <stdbool.h>
#include "retrosaga_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-specific functions
int bit_scaler_init(void);
int bit_scaler_process(void);
void bit_scaler_shutdown(void);
bool bit_scaler_validate(void);

#ifdef __cplusplus
}
#endif

#endif // BIT_SCALER_H
