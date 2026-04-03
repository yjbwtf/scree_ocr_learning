#ifndef DETECTOR_H
#define DETECTOR_H

#include <string>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>

namespace tesseract {
class TessBaseAPI;
}

struct DetectionResult {
    std::string text;
    cv::Rect bbox;
    float confidence = 0.0f; // 0..1
};

// Detector: wrapper around Tesseract OCR and basic OpenCV template matching
class Detector {
public:
    Detector();
    ~Detector();

    // Initialize the detector (e.g., start Tesseract). Returns true on success.
    bool init(const std::string &tessDataPath, const std::string &language = "eng");

    // Perform OCR on the provided image and return detected text blocks with bounding boxes.
    std::vector<DetectionResult> detectText(const cv::Mat &image);

    // Perform template matching and return vector of bounding boxes above the threshold.
    std::vector<cv::Rect> matchTemplate(const cv::Mat &image, const cv::Mat &templ, double threshold = 0.8);

private:
    std::unique_ptr<tesseract::TessBaseAPI> tess_;
};

#endif // DETECTOR_H
