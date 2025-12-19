#define UNICODE
#define _UNICODE
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h> // Dosya açmak için şart
#include <fstream>
#include <vector>
#include <string>
#include "../../include/flower_formats.h"

// Kütüphaneleri otomatik bağla (Derleme hatasını azaltır)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "shell32.lib") 

// Arayüz elemanları
#define ID_BUTTON_OPEN 1
#define ID_TEXT_OUTPUT 2

HWND hTitle, hButton, hOutput;
HFONT hFont;

// --- YARDIMCI: BMP Oluştur ve Aç ---
void CreateAndOpenBMP(const std::vector<unsigned char>& pixels, int w, int h) {
    wchar_t tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    std::wstring path = std::wstring(tempPath) + L"flower_temp_view.bmp";

    std::ofstream f(path.c_str(), std::ios::binary);
    if (!f) return;

    // Satırın 4'ün katı olması için gereken boşluk (Padding)
    int pad = (4 - (w * 3) % 4) % 4;
    int size = 54 + (w * 3 + pad) * h;

    unsigned char header[54] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0, 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    *(int*)&header[2] = size;
    *(int*)&header[18] = w;
    *(int*)&header[22] = -h; 

    f.write((char*)header, 54);

    // --- KRİTİK DÜZELTME: Satır Satır Yazma Döngüsü ---
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int i = (y * w + x) * 3;
            // Renkleri BGR olarak yaz (Windows standardı)
            f.put(pixels[i + 2]); // Blue
            f.put(pixels[i + 1]); // Green
            f.put(pixels[i]);     // Red
        }
        // Her satırın sonuna PADDING ekle (Kaymayı önleyen kısım)
        for (int k = 0; k < pad; k++) f.put(0);
    }
    
    f.close();
    ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

// --- DOSYA OKUMA MOTORU ---
void ReadFlowerFile(HWND hwnd) {
    OPENFILENAME ofn;       
    wchar_t szFile[260] = {0};    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Flower Files (*.ftf;*.fpf)\0*.ftf;*.fpf\0All Files\0*.*\0";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        std::ifstream file(szFile, std::ios::binary);
        if (!file) return;

        FlowerHeader header;
        file.read((char*)&header, sizeof(FlowerHeader));

        // İMZA KONTROLÜ
        if (strncmp(header.signature, "FLWR", 4) == 0) {
            
            // --- MOD 1: METİN (.ftf) ---
            if (header.fileType == 1) {
                file.seekg(0, std::ios::end);
                int len = (int)file.tellg() - sizeof(FlowerHeader);
                file.seekg(sizeof(FlowerHeader), std::ios::beg);
                
                std::vector<char> buf(len + 1);
                file.read(&buf[0], len);
                buf[len] = 0;

                // UTF-8 Dönüşümü (Bozuk karakter olmaması için)
                int wlen = MultiByteToWideChar(CP_UTF8, 0, &buf[0], len, 0, 0);
                std::wstring wstr(wlen, 0);
                MultiByteToWideChar(CP_UTF8, 0, &buf[0], len, &wstr[0], wlen);

                SetWindowText(hOutput, wstr.c_str());
                SetWindowText(hTitle, L"Flower Decoder v1.3 - Text Mode 📝");
            }
            // --- MOD 2: FOTOĞRAF (.fpf) ---
            else if (header.fileType == 2) {
                FPF_Body body;
                file.read((char*)&body, sizeof(FPF_Body));
                
                int pxSize = body.width * body.height * 3;
                std::vector<unsigned char> pixels(pxSize);
                file.read((char*)pixels.data(), pxSize);

                std::wstring msg = L"Flower Photo Format (.fpf)\r\n";
                msg += L"Resolution: " + std::to_wstring(body.width) + L"x" + std::to_wstring(body.height) + L"\r\n";
                msg += L"Status: Opening in external viewer...";
                
                SetWindowText(hOutput, msg.c_str());
                SetWindowText(hTitle, L"Flower Decoder v1.3 - Image Mode 📸");

                CreateAndOpenBMP(pixels, body.width, body.height);
            }
        } else {
            SetWindowText(hOutput, L"Error: Invalid File Signature (Not FLWR)");
        }
        file.close();
    }
}

// --- PENCERE AYARLARI ---
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_CREATE) {
        hFont = CreateFont(19,0,0,0,400,0,0,0,1,0,0,0,0,L"Segoe UI");
        
        hTitle = CreateWindow(L"STATIC", L"Flower Decoder v1.3", WS_VISIBLE|WS_CHILD|SS_CENTER, 20, 20, 740, 30, hwnd, 0, 0, 0);
        SendMessage(hTitle, WM_SETFONT, (WPARAM)hFont, 1);

        hButton = CreateWindow(L"BUTTON", L"Open File", WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 20, 60, 150, 40, hwnd, (HMENU)1, 0, 0);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, 1);

        hOutput = CreateWindow(L"EDIT", L"System Ready. Waiting for input...", WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE|ES_READONLY, 20, 110, 740, 420, hwnd, (HMENU)2, 0, 0);
        SendMessage(hOutput, WM_SETFONT, (WPARAM)hFont, 1);
    }
    else if (msg == WM_COMMAND && LOWORD(wp) == 1) ReadFlowerFile(hwnd);
    else if (msg == WM_DESTROY) PostQuitMessage(0);
    else return DefWindowProc(hwnd, msg, wp, lp);
    return 0;
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int) {
    SetProcessDPIAware(); // Yüksek çözünürlük için netlik ayarı
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hInstance = h;
    wc.lpszClassName = L"FLWR_V13_FINAL";
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);
    CreateWindow(L"FLWR_V13_FINAL", L"Flower Decoder v1.3", WS_VISIBLE|WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, 0, 0, h, 0);
    MSG m; while(GetMessage(&m,0,0,0)) { TranslateMessage(&m); DispatchMessage(&m); }
    return 0;
}