// src/main.cpp
#include <iostream>
#include <thread>
#include <chrono>
#include "screen_capture_gdi.h"
#include "detector.h"
#include "overlay_window.h"
#include "input_simulator.h"

int main(int argc, char** argv) {
    try {
        std::cout << "Starting screen OCR app\n";

        ScreenCaptureGDI capturer;
        if (!capturer.initialize()) {
            std::cerr << "Failed to initialize screen capture\n";
            return 1;
        }

        Detector detector;
        if (!detector.initialize()) {
            std::cerr << "Failed to initialize detector (Tesseract/OpenCV)\n";
            return 1;
        }

        OverlayWindow overlay;
        overlay.create("screen_ocr_overlay");

        InputSimulator input;

        const int interval_ms = 200; // capture interval
        for (;;) {
            auto frame = capturer.captureFrame();
            if (frame.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            auto results = detector.detect(frame);

            // Draw detection results on overlay
            overlay.clear();
            for (const auto& r : results) {
                overlay.drawRect(r.rect, /*color=*/{0,255,0,128});
                overlay.drawText(r.text, {r.rect.x, r.rect.y - 18}, /*color=*/{255,255,255,200});
            }
            overlay.present();

            // Example: if any result contains "OK", simulate a click (placeholder)
            for (const auto& r : results) {
                if (r.text.find("OK") != std::string::npos) {
                    input.clickAt(r.rect.x + r.rect.width/2, r.rect.y + r.rect.height/2);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << "\n";
        return 2;
    }
}