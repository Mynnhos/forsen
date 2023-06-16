#include "time.hpp"
#include <sstream>
#include <string>
#include <algorithm>
#include <format>

std::string time_to_str(int time)
{
    if(time < 0)
        return "NaN";
    int mm = (int)floor(time / 60);
    int ss = time - mm * 60;
    return std::format("{:02d}:{:02d}", mm, ss);
}

std::string time_to_excel_format(int time)
{
    if(time < 0)
        return "NaN";
    int hh = (int)floor(time / 3600);
    time -= hh * 3600;
    int mm = (int)floor(time / 60);
    int ss = time - mm * 60;
    return std::format("{:02d}:{:02d}:{:02d}", hh, mm, ss);
}

std::string time_to_url(int id, int time) {
    if(time < 0)
        return "NaN";
    int hh = (int)floor(time / 3600);
    time -= hh * 3600;
    int mm = (int)floor(time / 60);
    int ss = time - mm * 60;
    return std::format("https://www.twitch.tv/videos/{}?t={:02d}h{:02d}m{:02d}s", id, hh, mm, ss);
}

int time_to_int(std::string time)
{
    int hh, mm, ss;
    try {
        std::istringstream iss(time);
        std::string token;
        std::getline(iss, token, ':');  hh = std::stoi(token);
        std::getline(iss, token, ':');  mm = std::stoi(token);
        std::getline(iss, token);       ss = std::stoi(token);
    } catch(...) {
        return -1;
    }
    if(hh < 0 || mm < 0 || ss < 0 || mm > 59 || ss > 59 || hh > 99)
        return -1;
    return hh * 3600 + mm * 60 + ss;
}
