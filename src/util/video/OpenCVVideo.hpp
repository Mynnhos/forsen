#pragma once
#include "../Video.hpp"
#include <opencv2/highgui/highgui.hpp>

/// @brief A video reader implementation using OpenCV
class OpenCVVideo : public Video {
public:
    /// @brief Creates a new Video implementation from the given path
    /// @param path The location of the video file
    /// @param starting_time The starting time of the video. Useful if video is split into multiple files.
    OpenCVVideo(std::string path, float starting_time = 0);
    ~OpenCVVideo();

    const unsigned int get_width() const;
    const unsigned int get_height() const;
    const unsigned int get_fps() const;
    const unsigned int get_frame_position() const;
    /// @brief Get the number of frames in the video
    /// @return The number of frames in the video
    const unsigned int get_frame_count() const;
    /// @brief Get the current time in seconds
    /// @return The current time in seconds
    const float get_current_time() const;
    const bool is_open() const;

    /// @brief Stop the video and free resources
    void release();

    /// @brief Read the next frame
    /// @param frame The frame read (check frame.empty() to see if it's valid)
    /// @return True if successful
    bool read_frame(cv::Mat& frame);
    /// @brief Decode the next frame without converting it to RGB. Faster than read_frame().
    /// @return True if successful
    bool skip_frame();
    /// @brief Go to a specific frame
    /// @param frame The frame to go to
    /// @return True if successful
    bool go_to(int frame);

private:
    cv::VideoCapture capture;
    unsigned int width = 0, height = 0;
    unsigned int fps = 0;
    unsigned int f = 0;
    float t0 = 0;
};