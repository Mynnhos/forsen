#include "Run.hpp"
#include "util/time.hpp"

std::vector<Run> Run::runs = std::vector<Run>();

Run::Run(int vod_timestamp) : vod_timestamp(vod_timestamp)
{
    events = std::unordered_map<std::string, int>();
    played = false; ended = false;
    last_time = 0; spectator_switch_time = -1;
    is_suspicous = false;
    current_place = "overworld";
}

Run::~Run()
{
    // reset();
}

void Run::add_event(std::string event, int time)
{
    if(time < 0)
        return;
    events.emplace(event, time);
}

bool Run::has_event(std::string event)
{
    return events.find(event) != events.end();
}

void Run::set_suspicous(bool is_suspicous)
{
    this->is_suspicous = is_suspicous;
}

void Run::check_validity()
{
    // We didn't switch to spectator mode
    if(spectator_switch_time <= 0)
        return;
    // Remove elements if they happened ~after switching to spectator mode. False positives if spectator mode detected and forsen dies right when entering a bastion
    for(auto it = events.begin(); it != events.end(); ) {
        if(it->second > spectator_switch_time - 5) {
            it = events.erase(it);
        } else {
            ++it;
        }
    }
    printf("\tSwitched to spectator mode at %d, removed invalid events,\n", spectator_switch_time);
}

void Run::reset()
{
    check_validity();
    ended = true;
    printf("Run reset at %s\n", time_to_str(last_time).c_str());
    Run::add_run(*this);
}

void Run::add_run(Run &run)
{
    runs.push_back(run);
}

void Run::save_runs(std::string date, int vod_id, int stream_length)
{
    // TODO: Save runs to file
}
