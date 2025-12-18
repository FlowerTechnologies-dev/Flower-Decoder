#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include "../../include/flower_formats.h"

// Görüntü işleme kütüphanesini burada tanımlıyoruz
#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

// --- METİN (.ftf) OLUŞTURMA FONKSİYONU ---
void createFtfFile(const std::string& text, const std::string& outputPath) {
    std::ofstream file(outputPath, std::ios::binary);
    if (!file) {
        std::cerr << "Hata: Dosya olusturulamadi!" << std::endl;
        return;
    }

    // BURAYI GÜNCELLEDİK: FTF_Header yerine FlowerHeader kullanıyoruz
    FlowerHeader header; 
    memcpy(header.signature, "FLWR", 4);
    header.version = 1;
    header.fileType = 1; // 1: Metin (FTF)

    file.write((char*)&header, sizeof(FlowerHeader));
    
    // Metin boyutu bilgisini de bir şekilde saklamamız lazım. 
    // Basitlik için dosyanın geri kalanı direkt metin olacak.
    file.write(text.c_str(), text.length());
    file.close();

    std::cout << "Basariyla Metin dosyasi olusturuldu: " << outputPath << " 📝" << std::endl;
}

// --- FOTOĞRAF (.fpf) OLUŞTURMA FONKSİYONU ---
void createFpfFile(const std::string& inputImagePath, const std::string& outputFpfPath) {
    int width, height, channels;
    // Fotoğrafı RGB olarak yükle
    unsigned char* pixels = stbi_load(inputImagePath.c_str(), &width, &height, &channels, STBI_rgb);

    if (!pixels) {
        std::cerr << "Hata: Gorsel yuklenemedi! Yol dogru mu? -> " << inputImagePath << std::endl;
        return;
    }

    std::ofstream outFile(outputFpfPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Hata: Cikis dosyasi olusturulamadi!" << std::endl;
        stbi_image_free(pixels);
        return;
    }

    // Genel Header (v1.2 formatı için güncellenmiş mantık)
    // Not: flower_formats.h içinde FlowerHeader ve FPF_Body tanımlı olmalı
    FlowerHeader flowerHeader;
    memcpy(flowerHeader.signature, "FLWR", 4);
    flowerHeader.version = 1;
    flowerHeader.fileType = 2; // 2 = FPF (Fotoğraf)
    outFile.write((char*)&flowerHeader, sizeof(FlowerHeader));

    // FPF Body
    FPF_Body fpfBody;
    fpfBody.width = width;
    fpfBody.height = height;
    fpfBody.bitDepth = 24; // RGB
    outFile.write((char*)&fpfBody, sizeof(FPF_Body));

    // Piksel verilerini yaz (Genişlik * Yükseklik * 3 Bayt)
    outFile.write((char*)pixels, width * height * 3);

    outFile.close();
    stbi_image_free(pixels);
    std::cout << "Basariyla Fotoğraf dosyasi olusturuldu: " << outputFpfPath << " 📸" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Flower Maker v1.2" << std::endl;
        std::cout << "Kullanim:" << std::endl;
        std::cout << "  Metin icin:   maker create-ftf \"Mesajiniz\" cikis.ftf" << std::endl;
        std::cout << "  Fotoğraf icin: maker create-fpf kaynak.png cikis.fpf" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "create-ftf" && argc == 4) {
        createFtfFile(argv[2], argv[3]);
    } 
    else if (command == "create-fpf" && argc == 4) {
        createFpfFile(argv[2], argv[3]);
    } 
    else {
        std::cerr << "Gecersiz komut veya eksik parametre!" << std::endl;
        return 1;
    }

    return 0;
}