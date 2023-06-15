#include "matching.hpp"


TemplateMatcher igt_matcher = TemplateMatcher(8);


void configure_template_matchers()
{
    // igt_matcher = TemplateMatcher(8);
    igt_matcher.set_bounds(1166, 54, 1259, 71);
    igt_matcher.add_template("0", "forsen_720/0.png", 0.85f);
    igt_matcher.add_template("1", "forsen_720/1.png", 0.7f);
    igt_matcher.add_template("1", "forsen_720/1_2.png", 0.7f);
    igt_matcher.add_template("2", "forsen_720/2.png", 0.85f);
    igt_matcher.add_template("3", "forsen_720/3.png", 0.85f);
    igt_matcher.add_template("4", "forsen_720/4.png", 0.85f);
    igt_matcher.add_template("5", "forsen_720/5.png", 0.85f);
    igt_matcher.add_template("6", "forsen_720/6.png", 0.85f);
    igt_matcher.add_template("7", "forsen_720/7.png", 0.8f);
    igt_matcher.add_template("7", "forsen_720/7_2.png", 0.8f);
    igt_matcher.add_template("8", "forsen_720/8.png", 0.85f);
    igt_matcher.add_template("9", "forsen_720/9.png", 0.85f);
}

float get_time(cv::Mat &frame)
{
    std::vector<Match> matches = igt_matcher.get_matches(frame);
    
    if(matches.size() < 4) {
        return 0; // No timer on screen
    }
    if(matches.size() != 7) {
        return -1; // Error when trying to read the timer
    }

    int mm = 10*std::stoi(matches[0].name) + std::stoi(matches[1].name);
    int ss = 10*std::stoi(matches[2].name) + std::stoi(matches[3].name);
    int ms = 100*std::stoi(matches[4].name) + 10*std::stoi(matches[5].name) + std::stoi(matches[6].name);

    return mm*60+ss+ms/1000.0f;
}
