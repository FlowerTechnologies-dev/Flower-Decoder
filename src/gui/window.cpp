void ReadFlowerFile(HWND hwnd) {
    OPENFILENAME ofn;       
    wchar_t szFile[260];    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Flower Dosyaları\0*.ftf;*.fpf;*.fvf\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::ifstream file(ofn.lpstrFile, std::ios::binary);
        if (!file) return;

        FlowerHeader header;
        file.read((char*)&header, sizeof(FlowerHeader));

        if (strncmp(header.signature, "FLWR", 4) == 0) {
            // --- METİN (FTF) ---
            if (header.fileType == 1) {
                file.seekg(0, std::ios::end);
                int fileSize = (int)file.tellg() - sizeof(FlowerHeader);
                file.seekg(sizeof(FlowerHeader), std::ios::beg);

                std::vector<char> buffer(fileSize + 1);
                file.read(&buffer[0], fileSize);
                buffer[fileSize] = '\0';

                int size_needed = MultiByteToWideChar(CP_UTF8, 0, &buffer[0], fileSize, NULL, 0);
                std::wstring wstrTo(size_needed, 0);
                MultiByteToWideChar(CP_UTF8, 0, &buffer[0], fileSize, &wstrTo[0], size_needed);

                SetWindowText(hOutput, wstrTo.c_str());
                SetWindowText(hTitle, L"Flower Decoder - Text Mode");
            }
            // --- FOTOĞRAF (FPF) ---
            else if (header.fileType == 2) {
                FPF_Body imgBody;
                file.read((char*)&imgBody, sizeof(FPF_Body));

                // Bilgileri ekrana yazdır (Uyarı kutusu olmadan)
                std::wstring info = L"Flower Photo Detected\r\n";
                info += L"Resolution: " + std::to_wstring(imgBody.width) + L"x" + std::to_wstring(imgBody.height) + L"\r\n";
                info += L"Format: FPF (Flower Photo Format)\r\n";
                info += L"Status: Data ready for rendering.";

                SetWindowText(hOutput, info.c_str());
                SetWindowText(hTitle, L"Flower Decoder - Image Mode");

                // BURASI ÖNEMLİ: v1.3'te buraya direkt ekrana çizim kodu gelecek!
            }
        }
        file.close();
    }
}