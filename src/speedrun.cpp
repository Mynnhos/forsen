#include "speedrun.hpp"
#include <vector>
#include "util/video/OpenCVVideo.hpp"
#include "Run.hpp"
#include "matching.hpp"


/// @brief Check if the run is reset, is played, switched to spectator mode, ...
/// @param time The parsed time of the current frame
/// @param frame The current frame
/// @param run The current run
/// @return True if the run is still ongoing, false if we reset
bool check_run(float time, cv::Mat& frame, Run &run)
{
    if(time <= 0)
        return true;
    
    if(run.last_time >= 3 && (time > 0 && time < 3)) {
        run.reset();
        return false;
    }

    if(!run.played && time > 45)
        run.played = true;
    
    //TODO: Check if we switched to spectator mode

    run.last_time = (int)round(time);
    return true;
}


bool find_advancements(std::vector<std::string> advancements, std::unique_ptr<Video>& video, Run &run)
{
    cv::Mat frame;
    while(video->is_open()) {
        if(!video->read_frame(frame)) {
            break;
        }

        float time = get_time(frame);
        if(!check_run(time, frame, run)) {
            return false;
        }

        //TODO: Check if we find an advancement. If all found switch to next part

        for(int i = 0; i < 60/3; i++) {
            if(!video->skip_frame()) {
                video.release();
                break;
            }
        }
    }

    run.reset();

    return false;
}

void analyze_video(std::string path, int start_timestamp)
{
    configure_template_matchers();
    std::unique_ptr<Video> video = std::make_unique<OpenCVVideo>(path, start_timestamp);
    // OpenCVVideo video = OpenCVVideo(path, (float)start_timestamp);

    while(video->is_open()) {
        Run run((int)round(video->get_current_time()));

        if(!find_advancements({"nether"}, std::ref(video), run)) {
            continue;
        }
        
        if(!find_advancements({"bastion", "fortress"}, std::ref(video), run)) {
            continue;
        }

        //TODO: find nether exit

        //TODO: find stronghold entry

        //TODO: find end entry

        //TODO: find end of game
    }
}
