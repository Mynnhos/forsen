#pragma once
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/// @brief A template to match against.
struct Template {
    /// @brief Construct a new Template from its image and threshold.
    /// @param image The template image path.
    /// @param threshold The threshold to use when matching this template.
    Template(std::string image_path, float threshold) : image(cv::imread(image_path, cv::IMREAD_GRAYSCALE)), threshold(threshold) {}

    /// @brief The template image.
    cv::Mat image;
    /// @brief The threshold to use when matching this template.
    float threshold;
};