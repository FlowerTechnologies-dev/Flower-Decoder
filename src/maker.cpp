#include <iostream>
#include <fstream>
#include <string>
#include "../include/flower_formats.h"

int main() {
    // 1. Yazacağımız mesaj
    std::string message = "If you can read this message, test completed successfully!";
    
    // 2. Header (Başlık) bilgilerini dolduruyoruz
    FTF_Header myHeader;
    myHeader.signature[0] = 'F'; myHeader.signature[1] = 'L'; 
    myHeader.signature[2] = 'W'; myHeader.signature[3] = 'R';
    
    myHeader.fileType[0] = 'F'; myHeader.fileType[1] = 'T'; 
    myHeader.fileType[2] = 'F'; myHeader.fileType[3] = ' ';
    
    myHeader.version = 1;
    myHeader.fileSize = message.length();

    // 3. Dosyayı oluştur (Binary modda)
    std::ofstream outFile("assets/test.ftf", std::ios::binary);
    
    if (!outFile) {
        std::cout << "Dosya olusturulamadi! ❌" << std::endl;
        return 1;
    }

    // Önce başlığı yaz, sonra mesajı ekle
    outFile.write((char*)&myHeader, sizeof(FTF_Header));
    outFile.write(message.c_str(), message.length());

    outFile.close();
    std::cout << "Flower Text File (FTF) basariyla olusturuldu! 🌸✨" << std::endl;
    
    return 0;
}