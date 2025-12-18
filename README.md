# 🌸 Flower Decoder

![Version](https://img.shields.io/badge/version-v1.2--alpha-pink)
![License](https://img.shields.io/badge/license-GPL--3.0-blue)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)

**Flower Decoder** is a high-performance, lightweight file decoding engine and a core component of the **TwOS** ecosystem. It is specifically designed to handle proprietary Flower Technologies formats with maximum speed and minimum footprint.

## ✨ Key Features

* **Ultra-Lightweight:** Built with pure Win32 API to ensure near-zero system resource usage.
* **Proprietary Formats:**
    * `.ftf` (Flower Text Format) - Fully Supported.
    * `.fpf` (Flower Photo Format) - Header Decoding (Alpha).
    * `.fvf` (Flower Video Format) - Coming Soon.
* **High DPI Awareness:** Crystal-clear UI rendering on 4K and high-resolution displays.
* **TwOS Vision:** Engineered with the "Fast Boot" philosophy of the upcoming TwOS operating system.

## 🛠️ Build & Installation

The project is developed in **C++** using the **Win32 API**. You will need a C++ compiler (MinGW/g++ recommended).

```powershell
# Clone the repository
git clone [https://github.com/FlowerTechnologies-dev/Flower-Decoder.git](https://github.com/FlowerTechnologies-dev/Flower-Decoder.git)

# Compile the GUI Application
g++ src/gui/window.cpp -o build/gui_app.exe -mwindows -lcomdlg32
````

## 📸 Roadmap
**v1.1:** Stable GUI and Text (.ftf) integration.

**v1.2 Alpha:** Image (.fpf) header logic and format detection.

**v1.3 (Current Goal):** Internal pixel rendering engine (Native Image Viewer).

**v2.0:** Video (.fvf) playback support and TwOS File Explorer integration.

## 📜 License
This project is licensed under the GNU General Public License v3.0. We believe in open-source innovation under the Flower Technologies vision.

Developed with ❤️ by *Azeno42* (Flower Technologies)
