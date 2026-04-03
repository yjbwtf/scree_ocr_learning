#include "screen_capture_gdi.h"

#include <windows.h>
#include <stdexcept>
#include <algorithm>

ScreenCaptureGDI::ScreenCaptureGDI() = default;

ScreenCaptureGDI::~ScreenCaptureGDI() {
    shutdown();
}

bool ScreenCaptureGDI::initialize() {
    if (hScreenDC_ && hMemoryDC_) return true; // already

    hScreenDC_ = GetDC(nullptr); // primary screen
    if (!hScreenDC_) return false;

    hMemoryDC_ = CreateCompatibleDC(hScreenDC_);
    if (!hMemoryDC_) {
        ReleaseDC(nullptr, hScreenDC_);
        hScreenDC_ = nullptr;
        return false;
    }

    return true;
}

void ScreenCaptureGDI::shutdown() {
    if (hOldObject_) {
        SelectObject(hMemoryDC_, hOldObject_);
        hOldObject_ = nullptr;
    }
    if (hBitmap_) {
        DeleteObject(hBitmap_);
        hBitmap_ = nullptr;
    }
    if (hMemoryDC_) {
        DeleteDC(hMemoryDC_);
        hMemoryDC_ = nullptr;
    }
    if (hScreenDC_) {
        ReleaseDC(nullptr, hScreenDC_);
        hScreenDC_ = nullptr;
    }
}

bool ScreenCaptureGDI::captureFromDC(HDC srcDC, int x, int y, int width, int height, CaptureResult &result) {
    if (!srcDC || width <= 0 || height <= 0) return false;

    // Create a compatible bitmap
    HBITMAP hBmp = CreateCompatibleBitmap(srcDC, width, height);
    if (!hBmp) return false;

    HGDIOBJ old = SelectObject(hMemoryDC_, hBmp);
    if (!old) {
        DeleteObject(hBmp);
        return false;
    }

    // BitBlt from source DC into memory DC
    if (!BitBlt(hMemoryDC_, 0, 0, width, height, srcDC, x, y, SRCCOPY | CAPTUREBLT)) {
        SelectObject(hMemoryDC_, old);
        DeleteObject(hBmp);
        return false;
    }

    // Prepare BITMAPINFO for 32bpp output
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    size_t rowBytes = width * 4;
    result.pixels.resize(rowBytes * height);

    if (GetDIBits(hMemoryDC_, hBmp, 0, height, result.pixels.data(), &bmi, DIB_RGB_COLORS) == 0) {
        SelectObject(hMemoryDC_, old);
        DeleteObject(hBmp);
        return false;
    }

    // Fill result metadata
    result.width = width;
    result.height = height;

    // Cleanup
    SelectObject(hMemoryDC_, old);
    DeleteObject(hBmp);

    return true;
}

bool ScreenCaptureGDI::captureAll(CaptureResult &result) {
    if (!initialize()) return false;

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    return captureFromDC(hScreenDC_, 0, 0, width, height, result);
}

bool ScreenCaptureGDI::captureRegion(int left, int top, int right, int bottom, CaptureResult &result) {
    if (!initialize()) return false;
    if (right <= left || bottom <= top) return false;

    int width = right - left;
    int height = bottom - top;
    return captureFromDC(hScreenDC_, left, top, width, height, result);
}