//
// created by kacchan on 2024/3/27
//

#include "filters.h"
#include "../parser/arg_parser.h"

#include <iostream>
#include <map>

void CheckCountOfArgs(const ArgParser::FilterInfo& filter) {
    std::map<std::string, size_t> filter_args_count;
    filter_args_count["-neg"] = 0;
    filter_args_count["-crop"] = 2;
    filter_args_count["-gs"] = 0;
    filter_args_count["-sharp"] = 0;
    filter_args_count["-edge"] = 1;
    filter_args_count["-blur"] = 1;
    filter_args_count["-mc"] = 1;
    if (filter.args_.size() != filter_args_count[filter.name_]) {
        throw std::runtime_error("Wrong number of arguments");
    }
}

void ApplyFilters(const std::vector<ArgParser::FilterInfo>& filters, Image& image) {
    if (image.GetData().empty()) {
        std::cerr << "No image data to apply filters to." << std::endl;
        return;
    }
    for (auto& filter : filters) {
        CheckCountOfArgs(filter);
        if (filter.name_ == "-neg") {
            Negative neg_filter;
            neg_filter.Apply(image);
        } else if (filter.name_ == "-sharp") {
            Sharpening sharp_filter;
            sharp_filter.Apply(image);
        } else if (filter.name_ == "-crop") {
            int32_t width = std::stoi(filter.args_[0]);
            int32_t height = std::stoi(filter.args_[1]);
            Crop crop_filter(width, height);
            crop_filter.Apply(image);
        } else if (filter.name_ == "-gs") {
            Grayscale gs_filter;
            gs_filter.Apply(image);
        } else if (filter.name_ == "-edge") {
            double thresold = std::stod(filter.args_[0]);
            EdgeDetection edge_filter(thresold);
            edge_filter.Apply(image);
        } else if (filter.name_ == "-blur") {
            double sigma = std::stod(filter.args_[0]);
            GaussianBlur blur_filter(sigma);
            blur_filter.Apply(image);
        } else if (filter.name_ == "-mc") {
            int radius = std::stoi(filter.args_[0]);
            Smoothing mc_filter(radius);
            mc_filter.Apply(image);
        } else {
            std::cerr << "Unknown filter: " << filter.name_ << std::endl;
            return;
        }
    }
}
