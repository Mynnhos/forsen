#pragma once
#include "TemplateMatcher.hpp"


void configure_template_matchers();

float get_time(cv::Mat& frame);

std::vector<std::string> get_advancements(cv::Mat& frame);

bool is_loading_screen(cv::Mat& frame);