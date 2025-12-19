# 🌸 Flower Decoder v1.3 - Stable

The official decoder for **Flower Formats** (.ftf, .fpf). A core part of the future **TwOS** ecosystem by **Flower Technologies**.

## 🚀 What's New in v1.3?
- **Photo Support (.fpf):** Now you can open Flower Photo Files!
- **External Viewer Integration:** Images are automatically converted to BMP and opened in Windows Photos via `shell32`.
- **Enhanced Stability:** Based on the rock-solid Alpha text engine.
- **Security First:** Temporary files are handled in the system `Temp` folder for transparency.

## 🛠 Tech Stack
- **Language:** C++
- **UI:** Win32 API (Raw)
- **Compiler:** MinGW-w64 (GCC 15.2+)
- **OS:** Windows 10/11 (Optimized for Asus TUF Gaming Series)

## 📥 Installation & Compilation
```powershell
g++ src/gui/window.cpp -o build/gui_app.exe -mwindows -lcomdlg32 -lshell32
```

## ⚠️ Known Issues
- v1.0 .ftf files may have compatibility issues due to legacy header padding.
- Turkish character support is being reworked for v2.0.

Developed by *Azeno42* (Muratcan) Flower Technologies - "The Future of Smart Devices"
