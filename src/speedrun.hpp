#pragma once
#include <string>

bool find_advancements(std::vector<std::string> advancements, std::unique_ptr<Video> &video, Run &run);

void analyze_video(std::string path, int start_timestamp);
