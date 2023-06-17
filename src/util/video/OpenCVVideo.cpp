#include "OpenCVVideo.hpp"

OpenCVVideo::OpenCVVideo(std::string path, float starting_time) : Video(path)
{
    printf("Opening video at %s\n", path.c_str());
    capture = cv::VideoCapture(path);

    if(!capture.isOpened()) {
        printf("\tERROR: Failed to open the video !\n");
        return;
    }

    width = (unsigned int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
    height = (unsigned int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    fps = (unsigned int)round(capture.get(cv::CAP_PROP_FPS));
    f = 0;
    t0 = starting_time;

    printf("\tVideo of size (%u, %u) and fps=%u opened successfully !\n", width, height, fps);
}

OpenCVVideo::~OpenCVVideo()
{
    this->release();
}

const unsigned int OpenCVVideo::get_width() const
{
    return width;
}

const unsigned int OpenCVVideo::get_height() const
{
    return height;
}

const unsigned int OpenCVVideo::get_fps() const
{
    return fps;
}

const unsigned int OpenCVVideo::get_frame_position() const
{
    return f;
}

const unsigned int OpenCVVideo::get_frame_count() const
{
    return (unsigned int)capture.get(cv::CAP_PROP_FRAME_COUNT);
}

const float OpenCVVideo::get_current_time() const
{
    return t0 + (float)f/(float)fps;
}

const bool OpenCVVideo::is_open() const
{
    return capture.isOpened();
}

void OpenCVVideo::release()
{
    capture.release();
}

bool OpenCVVideo::read_frame(cv::Mat& frame)
{
    capture.read(frame);
    if(frame.empty())
        return false;
    f++;
    return true;
}

bool OpenCVVideo::skip_frame()
{
    bool successful = capture.grab();
    if(successful)
        f++;
    return successful;
}

bool OpenCVVideo::go_to(int frame)
{
    capture.set(cv::CAP_PROP_POS_FRAMES, frame);
    f = frame;
    return this->is_open();
}
