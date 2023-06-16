#include <iostream>
#include <algorithm>
#include <filesystem>

#include "util/args.hxx"
#include "util/time.hpp"
#include "speedrun.hpp"
#include "matching.hpp"
#include "Run.hpp"


bool download_vod(int vod_id, std::vector<int>& timestamps)
{
    bool success = true;
    for(int i = 0; i < timestamps.size() - 1; i++) {
        if(std::filesystem::exists(std::format("output/{}_{}.mp4", vod_id, i))) {
            printf("Not downloading segment %d (already downloaded)\n", i);
            continue;
        }
        int start = timestamps[i];
        int end = timestamps[i+1];
        printf("##### Downloading segment %d from %s to %s\n", i, time_to_excel_format(start).c_str(), time_to_excel_format(end).c_str());
        int ret = system(std::format("TwitchDownloaderCLI.exe videodownload --id {} --output output/{}_{}.mp4 --quality 720p60 -b {} -e {}",
                                        vod_id, vod_id, i, start, end).c_str());
        if(ret != 0)
            success = false;
        printf("\n");
    }
    return success;
}

int main(int argc, char **argv)
{
    args::ArgumentParser parser("forsen - A speedrun analysis tool for Twitch VODs", "");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag<int> vod_id(parser, "vod_id", "Twitch VOD ID", {'i', "id"});
    args::ValueFlag<std::string> vod_date(parser, "YYYY-MM-DD", "Twitch VOD date (YYYY-MM-DD)", {'d', "date"});
    args::Flag timestamps_flag(parser, "HH:MM:SS", "Timestamps of the beginning/end (and any number of intermediate cuts) (HH:MM:SS)", {'t', "times"});
    args::PositionalList <std::string> timestamps_str(parser, "HH:MM:SS", "HH:MM:SS timestamps");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    if(!vod_id || !vod_date || !timestamps_str || args::get(timestamps_str).size() < 2) {
        printf("Correct usage: --id VOD_ID --date YYYY-MM-DD --times HH:MM:SS [...] HH:MM:SS\n");
        if(!vod_id) printf("\tMissing VOD ID\n");
        if(!vod_date) printf("\tMissing VOD date\n");
        if(!timestamps_str) printf("\tMissing timestamps\n");
        if(args::get(timestamps_str).size() < 2) printf("\tNeed at least 2 timestamps\n");
        return -1;
    }

    std::vector<int> timestamps;
    for(std::string timestamp : args::get(timestamps_str)) {
        int time = time_to_int(timestamp);
        if(time < 0) {
            printf("Invalid timestamp: %s\n", timestamp.c_str());
            return -1;
        }
        timestamps.push_back(time);
    }
    
    if(!std::is_sorted(timestamps.begin(), timestamps.end())) {
        printf("Timestamps must be in order\n");
        return -1;
    }


    printf("\n!===== forsenLevel !\n");
    printf("Analyzing VOD %d from %s. Timestamps: ", args::get(vod_id), args::get(vod_date).c_str());
    for(int timestamp : timestamps) {
        printf("%s ", time_to_excel_format(timestamp).c_str());
    }
    printf(".\n\n");


    if(!download_vod(args::get(vod_id), timestamps)) {
        printf("Failed to download VOD\n");
        return -1;
    }
    
    clock_t start, end;
    start = clock();

    configure_template_matchers();
    for(int i = 0; i < timestamps.size() - 1; i++) {
        printf("\n##### Analyzing segment %d from %s to %s\n", i, time_to_excel_format(timestamps[i]).c_str(), time_to_excel_format(timestamps[i+1]).c_str());
        std::string filename = std::format("output/{}_{}.mp4", args::get(vod_id), i);
        analyze_video(filename, timestamps[i]);
    }

    Run::save_runs(args::get(vod_date), args::get(vod_id), timestamps.back() - timestamps.front());

    end = clock();

    double time_taken = double(end-start)/double(CLOCKS_PER_SEC);
    printf("Time taken by program is : %.2f sec\n", time_taken);

    return 0;
}