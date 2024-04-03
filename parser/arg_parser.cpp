//
// created by kacchan on 2024/3/27
//

#include "arg_parser.h"
#include <iostream>

ArgParser::ArgParser(int argc, char** argv) {
    if (argc < 3) {
        throw std::runtime_error("Not enough args!");
    }
    input_file_ = argv[1];
    output_file_ = argv[2];

    FilterInfo current_filter;

    for (int i = 3; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (!current_filter.name_.empty()) {
                filters_.push_back(current_filter);
                current_filter.args_.clear();
            }
            current_filter.name_ = argv[i];
        } else {
            current_filter.args_.push_back(argv[i]);
        }
    }
    if (!current_filter.name_.empty()) {
        filters_.push_back(current_filter);
    }
}