#define UNICODE
#define _UNICODE
#include <windows.h>
#include <commdlg.h>
#include <fstream>
#include <string>
#include <vector>
#include "../../include/flower_formats.h"

#pragma comment(lib, "user32.lib")

#define ID_BUTTON_OPEN 1
#define ID_TEXT_OUTPUT 2

HWND hTitle, hButton, hOutput;
HFONT hFont;

void ReadFlowerFile(HWND hwnd) {
    OPENFILENAME ofn;       
    wchar_t szFile[260];    

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    
    // Filtre kısmında Türkçe karakterler ve gelecek formatlar
    ofn.lpstrFilter = L"Flower Dosyaları (*.ftf, *.fpf, *.fvf)\0*.ftf;*.fpf;*.fvf\0Flower Metin (*.ftf)\0*.ftf\0Flower Fotoğraf (*.fpf)\0*.fpf\0Flower Video (*.fvf)\0*.fvf\0";
    
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::ifstream file(ofn.lpstrFile, std::ios::binary);
        if (!file) {
            SetWindowText(hOutput, L"Hata: Dosya açılamadı!");
            return;
        }

        FTF_Header header;
        file.read((char*)&header, sizeof(FTF_Header));

        std::string sig(header.signature, 4);
        if (sig == "FLWR") {
            std::vector<char> buffer(header.fileSize + 1);
            file.read(&buffer[0], header.fileSize);
            buffer[header.fileSize] = '\0';

            int size_needed = MultiByteToWideChar(CP_UTF8, 0, &buffer[0], (int)header.fileSize, NULL, 0);
            std::wstring wstrTo(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, &buffer[0], (int)header.fileSize, &wstrTo[0], size_needed);

            SetWindowText(hOutput, wstrTo.c_str());
            SetWindowText(hTitle, L"Flower Decoder - Dosya Başarıyla Okundu! 🌸");
        } else {
            SetWindowText(hOutput, L"Hata: Geçersiz Flower dosyası imzası!");
        }
        file.close();
    }
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            // Font ve arayüz elemanlarında Türkçe desteği
            hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, TURKISH_CHARSET, 
                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

            hTitle = CreateWindow(L"STATIC", L"Flower Decoder v1.1 - Dosya Bekleniyor...",
                WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 20, 740, 30, hwnd, NULL, NULL, NULL);
            SendMessage(hTitle, WM_SETFONT, (WPARAM)hFont, TRUE);

            hButton = CreateWindow(L"BUTTON", L"Dosya Seç",
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 60, 150, 40, hwnd, (HMENU)ID_BUTTON_OPEN, NULL, NULL);
            SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

            hOutput = CreateWindow(L"EDIT", L"Lütfen bir Flower dosyası (.ftf, .fpf, .fvf) seçin...",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
                20, 110, 740, 400, hwnd, (HMENU)ID_TEXT_OUTPUT, NULL, NULL);
            SendMessage(hOutput, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON_OPEN) ReadFlowerFile(hwnd);
            break;

        case WM_CLOSE: DestroyWindow(hwnd); break;
        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int nCmdShow) {
    SetProcessDPIAware(); 

    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInstance;
    wc.lpszClassName = L"FlowerUI_v1_1";
    wc.lpfnWndProc = WindowProcedure;
    if (!RegisterClass(&wc)) return -1;

    HWND hwnd = CreateWindow(L"FlowerUI_v1_1", L"Flower Decoder v1.1",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}