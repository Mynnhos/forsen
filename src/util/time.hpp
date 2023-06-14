#pragma once
#include <string>

std::string time_to_str(int time);

std::string time_to_excel_format(int time);

std::string time_to_url(int id, int time);

int time_to_int(std::string time);