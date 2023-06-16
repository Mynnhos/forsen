#include "Run.hpp"
#include <format>
#include <fstream>
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


//TODO: Find a better way (method) to format the output string
std::string format_event(std::unordered_map<std::string, int>& map, std::string key)
{
    auto it = map.find(key);
    if(it == map.end())
        return "";
    return time_to_excel_format(it->second);
}

template<typename T>
int get_or_default(std::unordered_map<std::string, T>& map, std::string key, T default_value)
{
    auto it = map.find(key);
    if(it == map.end())
        return default_value;
    return it->second;
}


void Run::save_runs(std::string date, int vod_id, int stream_length)
{
    printf("Saving runs. Stream length: %s\n", time_to_excel_format(stream_length).c_str());
    
    std::string filename = std::format("output/{}_{}.csv", date, vod_id);
    std::ofstream csv(filename, std::ofstream::trunc);
    if(!csv.is_open()) {
        printf("Failed to open output .csv file %s\n", filename.c_str());
        return;
    }

    csv << "id" << "," << "date" << "," << "type" << "," << "deathCause" << "," << "deathMob" << "," << "endPlace" << ","
        << "netherEntry" << "," << "structure1" << "," << "structure1Entry" << "," << "structure2" << "," << "structure2Entry"<< ","
        << "netherExit" << "," << "strongholdEntry" << "," << "endEntry" << "," << "ending" << ","
        << "resetAt" << "," << "suspicious" << "," << "comment" << "," << "VOD link" << "," << "Stream length" << "\n";

    for (int i = 0; i < runs.size(); i++) {
        Run run = runs[i];

        // Find the structure that was entered first
        std::string structure1 = "";
        std::string structure2 = "";
        if(get_or_default(run.events, std::string("bastion"), 9999) < get_or_default(run.events, "fortress", 9999))
            structure1 = "bastion";
        if(get_or_default(run.events, "fortress", 9999) < get_or_default(run.events, "bastion", 9999))
            structure1 = "fortress";
        if(get_or_default(run.events, "bastion", 0) > get_or_default(run.events, "fortress", 9999))
            structure2 = "bastion";
        if(get_or_default(run.events, "fortress", 0) > get_or_default(run.events, "bastion", 9999))
            structure2 = "fortress";

        csv << i << "," << date << "," << "" << "," << "" << "," << "" << "," << run.current_place << ","
            << format_event(run.events, "nether") << ","
            << structure1 << "," << format_event(run.events, structure1) << ","
            << structure2 << "," << format_event(run.events, structure2) << ","
            << format_event(run.events, "nether_exit") << ","
            << format_event(run.events, "stronghold") << ","
            << format_event(run.events, "end") << ","
            << format_event(run.events, "ending") << ","
            << time_to_excel_format(run.last_time) << "," << run.is_suspicous << "," << "" << ","
            << time_to_url(vod_id, run.vod_timestamp) << ","
            << ((i==0)?time_to_excel_format(stream_length):"") << "\n";
    }
    csv.close();
}
