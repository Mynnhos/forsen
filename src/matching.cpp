#include "matching.hpp"


TemplateMatcher igt_matcher = TemplateMatcher(8);
TemplateMatcher adv_matcher = TemplateMatcher(40);
TemplateMatcher loading_matcher = TemplateMatcher(40);


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

    adv_matcher.set_bounds(1019, 31, 1055, 200);
    adv_matcher.add_template("nether", "forsen_720/adv_nether.png", 0.7f);
    adv_matcher.add_template("bastion", "forsen_720/adv_bastion.png", 0.7f);
    adv_matcher.add_template("fortress", "forsen_720/adv_fortress.png", 0.7f);
    adv_matcher.add_template("stronghold", "forsen_720/adv_stronghold.png", 0.7f);
    adv_matcher.add_template("end", "forsen_720/adv_end.png", 0.7f);

    loading_matcher.set_bounds(510, 258, 610, 278);
    loading_matcher.add_template("loading", "forsen_720/loading_screen_1.png", 0.7f);
    loading_matcher.add_template("loading", "forsen_720/loading_screen_2.png", 0.7f);
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

std::vector<std::string> get_advancements(cv::Mat &frame)
{
    // Get matches and return a list of the names
    std::vector<Match> matches = adv_matcher.get_matches(frame);
    std::vector<std::string> advs;
    for(auto match : matches) {
        advs.push_back(match.name);
    }
    return advs;
}

bool is_loading_screen(cv::Mat &frame)
{
    return loading_matcher.get_matches(frame).size() > 0;
}
