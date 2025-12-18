#ifndef FLOWER_FORMATS_H
#define FLOWER_FORMATS_H

#include <stdint.h>

// --- GENEL FLOWER HEADER ---
struct FlowerHeader {
    char signature[4]; // "FLWR"
    uint8_t version;   // 1
    uint8_t fileType;  // 1: Metin, 2: Fotoğraf, 3: Video
};

// --- FPF (Flower Photo Format) ---
struct FPF_Body {
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;  // 24 (RGB) veya 32 (RGBA)
    // Ardından pikseller gelecek: [R][G][B] [R][G][B] ...
};

// --- FVF (Flower Video Format) ---
struct FVF_Body {
    uint32_t width;
    uint32_t height;
    uint32_t frameCount;
    uint16_t fps;      // Kare hızı (Örn: 30 veya 60)
    // Ardından her kare için piksel verileri gelecek
};

#endif