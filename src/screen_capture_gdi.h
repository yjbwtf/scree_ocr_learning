#ifndef SCREEN_CAPTURE_GDI_H
#define SCREEN_CAPTURE_GDI_H

#include <windows.h>
#include <vector>
#include <cstdint>

// Simple result container for captured bitmap data
struct CaptureResult {
    int width;
    int height;
    std::vector<uint8_t> pixels; // BGR(BGRX) or BGRA depending on usage
};

class ScreenCaptureGDI {
public:
    ScreenCaptureGDI();
    ~ScreenCaptureGDI();

    // Initialize internal GDI objects. Returns true on success.
    bool initialize();

    // Capture the entire primary desktop into result. Returns true on success.
    bool captureAll(CaptureResult &result);

    // Capture a region (left, top, right, bottom in screen coordinates)
    bool captureRegion(int left, int top, int right, int bottom, CaptureResult &result);

    // Release any allocated resources.
    void shutdown();

private:
    HDC hScreenDC_ = nullptr;
    HDC hMemoryDC_ = nullptr;
    HBITMAP hBitmap_ = nullptr;
    HGDIOBJ hOldObject_ = nullptr;

    // Internal helper to perform a capture from a source HDC
    bool captureFromDC(HDC srcDC, int x, int y, int width, int height, CaptureResult &result);
};

#endif // SCREEN_CAPTURE_GDI_H