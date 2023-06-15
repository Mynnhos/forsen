#pragma once
#include <string>
#include <vector>
#include <memory>
#include "util/video/OpenCVVideo.hpp"
#include "Run.hpp"


bool find_advancements(std::vector<std::string> advancements, std::unique_ptr<Video> &video, Run &run);

bool find_loading_screen(std::string event, std::unique_ptr<Video> & video, Run &run);

void analyze_video(std::string path, int start_timestamp);
