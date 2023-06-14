#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "Template.hpp"


struct Match {
    std::string name = "";
    int x = 0, y = 0;
    float confidence = 0.0f;

    Match(std::string name, int x, int y, float confidence = 0.0f) : name(name), x(x), y(y), confidence(confidence) {}
};

/// @brief A class containing a set of templates and a method to find matches in an image
class TemplateMatcher {
public:
    /// @brief Construct a new Template Matcher object
    /// @param min_match_distance The minimum distance (in pixels) between two matches
    TemplateMatcher(int min_match_distance = 3);

    /// @brief Sets part part of the image to search for matches in (in pixels)
    void set_bounds(int xmin, int ymin, int xmax, int ymax);
    /// @brief Sets part part of the image to search for matches in (in percentage of image)
    void set_bounds(float xmin, float ymin, float xmax, float ymax, int width, int height);
    /// @brief Sets the minimum distance (in pixels) between two matches
    /// @param min_match_distance The minimum distance (in pixels) between two matches
    void set_min_match_distance(int min_match_distance);

    /// @brief Adds a new template to the matcher from its path
    /// @param name The name of the template (e.g. a letter, an object, etc.)
    /// @param image_path The path to the image of the template
    /// @param threshold How close the match has to be to the template to be considered a match
    /// @return True if the template was added successfully, false otherwise
    bool add_template(std::string name, std::string image_path, float threshold);
    /// @brief Get the matches in the image and return a filtered list (set bounds first!)
    /// @param image The image to search for matches in
    /// @return The filtered list of matches
    std::vector<Match> get_matches(cv::Mat& image);

private:
    /// @brief Filters the matches to remove duplicates and matches that are too close to each other
    /// @param matches The matches to filter
    void filter_matches(std::vector<Match>& matches);

    std::unordered_map<std::string, std::vector<Template>> template_groups;
    int min_match_distance = 3;
    int xmin = -1, xmax = -1, ymin = -1, ymax = -1;
};