#pragma once
#include <string>
#include <opencv2/core/core.hpp>

/// @brief Abstract class for video reading, see /video for implementations
class Video {
public:
    Video(std::string path) {};
    ~Video() {};

    virtual const unsigned int get_width() const = 0;
    virtual const unsigned int get_height() const = 0;
    virtual const unsigned int get_fps() const = 0;
    virtual const unsigned int get_frame_position() const = 0;
    /// @brief Get the number of frames in the video
    /// @return The number of frames in the video
    virtual const unsigned int get_frame_count() const = 0;
    /// @brief Get the current time in seconds
    /// @return The current time in seconds
    virtual const float get_current_time() const = 0;
    virtual const bool is_open() const = 0;

    /// @brief Stop the video and free resources
    virtual void release() = 0;

    /// @brief Read the next frame
    /// @param frame The frame read (check frame.empty() to see if it's valid)
    /// @return True if successful
    virtual bool read_frame(cv::Mat& frame) = 0;
    /// @brief Decode the next frame without converting it to RGB. Faster than read_frame().
    /// @return True if successful
    virtual bool skip_frame() = 0;
    /// @brief Go to a specific frame
    /// @param frame The frame to go to
    /// @return True if successful
    virtual bool go_to(int frame) = 0;
};