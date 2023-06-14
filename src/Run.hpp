#pragma once
#include <vector>
#include <unordered_map>


class Run {
public:
    Run(int vod_timestamp);
    ~Run();

    void add_event(std::string event, int time);
    void set_suspicous(bool is_suspicous);

    void reset();

    static void add_run(Run &run);
    static void save_runs(std::string date, int vod_id, int stream_length);

    bool ended;
    /// @brief Last IGT time parsed in seconds
    int last_time = 0;
    /// @brief Last IGT time in a valid run (before switching to spectator mode)
    int spectator_switch_time = 0;
    /// @brief Is/has the run been played (>3s)
    bool played;
    std::string current_place;
private:
    /// @brief A list of events (by name) and their IGT time (in seconds)
    std::unordered_map<std::string, int> events;

    /// @brief VOD timestamp to the beginning of the run
    int vod_timestamp;
    /// @brief Should the run be manually checked
    bool is_suspicous;

    static std::vector<Run> runs;

    /// @brief Remove invalid events (e.g. after switching to spectator mode)
    void check_validity();
};