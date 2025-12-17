#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/flower_formats.h"

int main() {
    // 1. Oluşturduğumuz FTF dosyasını açıyoruz
    std::ifstream file("assets/test.ftf", std::ios::binary);

    if (!file) {
        std::cout << "[Error] test.ftf not found! ❌" << std::endl;
        return 1;
    }

    // 2. Header yapımızı FTF olarak güncelliyoruz
    FTF_Header header;
    file.read((char*)&header, sizeof(FTF_Header));

    // 3. İmza kontrolü
    std::string sig(header.signature, 4);

    std::cout << "\n--- Flower Decoder V1.0 ---" << std::endl;
    
    if (sig == "FLWR") {
        std::cout << "Status: Flower Format Recognized! 🌸" << std::endl;
        std::cout << "File Version: V" << header.version << ".0" << std::endl;

        // 4. Mesajı okuma kısmı (Asıl sihir burada)
        // Dosyadaki metni okumak için bir alan (buffer) oluşturuyoruz
        std::vector<char> buffer(header.fileSize + 1);
        file.read(&buffer[0], header.fileSize);
        buffer[header.fileSize] = '\0'; // Metnin bittiğini belirtmek için

        std::cout << "Message: " << &buffer[0] << std::endl;
        std::cout << "---------------------------\n" << std::endl;
    } else {
        std::cout << "Status: Invalid File Format! ⚠️" << std::endl;
    }

    file.close();
    return 0;
}