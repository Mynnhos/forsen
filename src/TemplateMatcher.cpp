#include "TemplateMatcher.hpp"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

TemplateMatcher::TemplateMatcher(int min_match_distance)
{
    template_groups = std::unordered_map<std::string, std::vector<Template>>();
    this->min_match_distance = min_match_distance;
}

void TemplateMatcher::set_bounds(int xmin, int ymin, int xmax, int ymax)
{
    this->xmin = xmin; this->xmax = xmax; this->ymin = ymin; this->ymax = ymax;
}

void TemplateMatcher::set_bounds(float xmin, float ymin, float xmax, float ymax, int width, int height)
{
    if(xmin < 0 || xmax > 1 || ymin < 0 || ymax > 1) throw std::invalid_argument("Bounds must be between 0 and 1");
    this->xmin = (int)roundf(xmin * width); this->xmax = (int)roundf(xmax * width);
    this->ymin = (int)roundf(ymin * height); this->ymax = (int)roundf(ymax * height);
}

void TemplateMatcher::set_min_match_distance(int min_match_distance)
{
    this->min_match_distance = min_match_distance;
}

bool TemplateMatcher::add_template(std::string name, std::string image_path, float threshold)
{
    if(template_groups.find(name) == template_groups.end()) {
        template_groups[name] = std::vector<Template>();
    }
    template_groups[name].push_back(Template(image_path, threshold));
    return !template_groups[name].back().image.empty();
}

std::vector<Match> TemplateMatcher::get_matches(cv::Mat &image)
{
    // Check that all bounds are set, otherwise print an error message and return an empty vector
    if(xmin == -1 || xmax == -1 || ymin == -1 || ymax == -1) {
        std::cerr << "Bounds not set!" << std::endl;
        return std::vector<Match>();
    }

    // Crop the image to the bounds (without copying the data)
    cv::Mat cropped = image(cv::Rect(xmin, ymin, xmax - xmin, ymax - ymin));
    cv::Mat bw_cropped;
    cv::cvtColor(cropped, bw_cropped, cv::COLOR_BGR2GRAY);

    std::vector<Match> matches = std::vector<Match>();

    // Loop through all templates, do template matching (with threshold) and add all matches to the matches vector
    cv::Mat result;
    for(const auto& [name, templates] : template_groups) {
        for(auto& template_ : templates) {
            cv::matchTemplate(bw_cropped, template_.image, result, cv::TM_CCOEFF_NORMED);
            // Add all matches above the threshold to the matches vector
            for(int i = 0; i < result.rows; i++) {
                for(int j = 0; j < result.cols; j++) {
                    if(result.at<float>(i, j) >= template_.threshold) {
                        matches.push_back(Match(name, j, i, result.at<float>(i, j)));
                    }
                }
            }
        }
    }
    // Remove duplicates and matches that are too close to each other
    this->filter_matches(matches);this->filter_matches(matches);

    return matches;
}

void TemplateMatcher::filter_matches(std::vector<Match> &matches)
{
    if(matches.size() <= 1) return;
    //TODO: maybe add a "smarter" way to filter matches based on the match value
    for(int i = 0; i < matches.size(); i++) {
        for(int j = i + 1; j < matches.size(); j++) {
            if(matches[i].name == matches[j].name) {
                if(abs(matches[i].x - matches[j].x) < min_match_distance && abs(matches[i].y - matches[j].y) < min_match_distance) {
                    matches.erase(matches.begin() + j);
                    j--;
                }
            }
        }
    }
}
