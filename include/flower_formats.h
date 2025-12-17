#ifndef FLOWER_FORMATS_H
#define FLOWER_FORMATS_H

// Flower Text File (FTF) - Sürüm 1.0
struct FTF_Header {
    char signature[4];     // "FLWR"
    char fileType[4];      // "FTF " (Metin dosyası olduğunu belirtir)
    int version;           // 1
    int fileSize;          // Dosyanın içindeki metnin karakter sayısı
};

#endif