#!/bin/bash
# Comprehensive Audio Subsystem Build Script
# Aegis Project Phase 1 Implementation

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build/audio"
SRC_DIR="$PROJECT_ROOT/src/audio"
INCLUDE_DIR="$PROJECT_ROOT/include"
OUT_DIR="$PROJECT_ROOT/bin/audio"

# Color output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m'

log_info() {
    echo -e "${BLUE}[AUDIO_BUILD]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[AUDIO_BUILD]${NC} $1"
}

log_error() {
    echo -e "${RED}[AUDIO_BUILD]${NC} $1"
}

# Create necessary directories
mkdir -p "$BUILD_DIR" "$OUT_DIR"

# Define source modules in dependency order
INPUT_MODULES=(
    "input_audio.c"
    "audio_entropy.c"
    "prng_module.c"
)

PROCESSING_MODULES=(
    "bit_scaler.c"
    "midi_processing.c"
    "effect_engine.c"
)

OUTPUT_MODULES=(
    "waveform_generator.c"
    "sound_output.c"
)

ALL_MODULES=("${INPUT_MODULES[@]}" "${PROCESSING_MODULES[@]}" "${OUTPUT_MODULES[@]}")

# Compiler flags with enhanced audio support
CFLAGS="-std=c99 -Wall -Werror -O2 -I$INCLUDE_DIR"
LDFLAGS="-lm -lpthread"

# Add audio library support if available
if pkg-config --exists alsa; then
    CFLAGS="$CFLAGS $(pkg-config --cflags alsa)"
    LDFLAGS="$LDFLAGS $(pkg-config --libs alsa)"
    log_info "ALSA support enabled"
fi

if pkg-config --exists libpulse; then
    CFLAGS="$CFLAGS $(pkg-config --cflags libpulse)"
    LDFLAGS="$LDFLAGS $(pkg-config --libs libpulse)"
    log_info "PulseAudio support enabled"
fi

OUTPUT_EXEC="$OUT_DIR/retrosaga_audio_test"

# Compile each module
OBJECTS=()
log_info "Compiling audio modules..."

for FILE in "${ALL_MODULES[@]}"; do
    SRC_PATH="$SRC_DIR/$FILE"
    OBJ_PATH="$BUILD_DIR/${FILE%.c}.o"
    
    if [[ -f "$SRC_PATH" ]]; then
        log_info "Compiling $FILE..."
        gcc $CFLAGS -c "$SRC_PATH" -o "$OBJ_PATH"
        OBJECTS+=("$OBJ_PATH")
    else
        log_error "Source file not found: $SRC_PATH"
        exit 1
    fi
done

# Create simple test main for audio subsystem
cat > "$BUILD_DIR/audio_main.c" << 'EOL'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio/retrosaga_audio.h"

int main(int argc, char* argv[]) {
    printf("=== RetroSaga Audio Subsystem Test ===\n");
    
    bool diagnose_mode = (argc > 1 && strcmp(argv[1], "--diagnose") == 0);
    
    // Initialize audio subsystem
    if (retrosaga_audio_init() != RETROSAGA_SUCCESS) {
        printf("ERROR: Failed to initialize audio subsystem\n");
        return 1;
    }
    
    if (diagnose_mode) {
        printf("Running audio subsystem diagnostics...\n");
        
        if (!retrosaga_audio_validate()) {
            printf("ERROR: Audio subsystem validation failed\n");
            retrosaga_audio_shutdown();
            return 1;
        }
        
        printf("All audio modules validated successfully\n");
    } else {
        printf("Audio subsystem initialized successfully\n");
        printf("Processing audio for 5 seconds...\n");
        
        // Simulate audio processing
        for (int i = 0; i < 300; i++) { // 5 seconds at 60 FPS
            retrosaga_audio_update(16.67f);
            usleep(16670); // ~60 FPS
        }
    }
    
    retrosaga_audio_shutdown();
    printf("Audio subsystem test complete\n");
    return 0;
}
EOL

# Compile test main
gcc $CFLAGS -c "$BUILD_DIR/audio_main.c" -o "$BUILD_DIR/audio_main.o"
OBJECTS+=("$BUILD_DIR/audio_main.o")

# Link objects into final binary
log_info "Linking final executable..."
gcc "${OBJECTS[@]}" -o "$OUTPUT_EXEC" $LDFLAGS

log_success "Audio system compiled: $OUTPUT_EXEC"

# Validation steps
if [[ -f "$OUTPUT_EXEC" ]]; then
    log_info "Executable exists. Running diagnostics..."
    if "$OUTPUT_EXEC" --diagnose; then
        log_success "Audio subsystem diagnostics passed"
    else
        log_error "Audio subsystem diagnostics failed"
        exit 1
    fi
else
    log_error "Build failed - executable not created"
    exit 1
fi

log_success "Audio subsystem build and validation complete"
