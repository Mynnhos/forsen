#include "speedrun.hpp"
#include "util/time.hpp"
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



bool is_advancement_legit(std::string advancement, Run &run)
{
    if(advancement == "bastion" && run.current_place != "nether") return false;
    if(advancement == "fortress" && run.current_place != "nether") return false;
    if(advancement == "nether_exit" && (!run.has_event("bastion") || !run.has_event("fortress"))) return false;
    if(advancement == "stronghold" && !run.has_event("nether_exit")) return false;
    if(advancement == "end" && !run.has_event("stronghold")) return false;

    return true;
}

bool find_advancements(std::vector<std::string> advancements, std::unique_ptr<Video>& video, Run &run)
{
    cv::Mat frame;
    while(video->is_open()) {
        if(!video->read_frame(frame)) {
            video.release();
            break;
        }

        float time = get_time(frame);
        if(!check_run(time, frame, run)) {
            return false;
        }

        std::vector advs = get_advancements(frame);
        for(auto adv : advs) {
            if(std::find(advancements.begin(), advancements.end(), adv) == advancements.end() || run.has_event(adv))
                continue;
            
            if(!is_advancement_legit(adv, run))
                continue;
            
            printf("\tGot advancement %s at %s\n", adv.c_str(), time_to_str(run.last_time).c_str());
            run.add_event(adv, run.last_time-1); //1s delay to account for the advancement animation
            if(adv == "nether") run.current_place = "nether";
            else if(adv == "end") run.current_place = "end";

            advancements.erase(std::remove(advancements.begin(), advancements.end(), adv), advancements.end());
        }

        if(advancements.empty())
            return true;
        

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

bool find_loading_screen(std::string event, std::unique_ptr<Video> &video, Run &run)
{
    cv::Mat frame;
    while(video->is_open()) {
        if(!video->read_frame(frame)) {
            video.release();
            break;
        }

        float time = get_time(frame);
        if(!check_run(time, frame, run)) {
            return false;
        }
        
        if(is_loading_screen(frame)) {
            printf("\tGot %s at %s\n", event.c_str(), time_to_str(run.last_time).c_str());
            run.add_event(event, run.last_time);
            return true;
        }

        for(int i = 0; i < 2; i++) {
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
    std::unique_ptr<Video> video = std::make_unique<OpenCVVideo>(path, start_timestamp);

    while(video->is_open()) {
        printf("Starting new run\n");
        Run run((int)round(video->get_current_time()));

        if(!find_advancements({"nether"}, std::ref(video), run)) {
            continue;
        }
        
        if(!find_advancements({"bastion", "fortress"}, std::ref(video), run)) {
            continue;
        }

        if(!find_loading_screen("nether_exit", std::ref(video), run)) {
            continue;
        }

        if(!find_advancements({"stronghold"}, std::ref(video), run)) {
            continue;
        }

        if(!find_advancements({"end"}, std::ref(video), run)) {
            continue;
        }

        //TODO: find end of game
    }
    printf("Finished analyzing video\n");
}
