//
// created by kacchan on 2024/3/27
//

#pragma once

#include <vector>
#include <string>

class ArgParser {
public:
    struct FilterInfo {
        std::string name_;
        std::vector<std::string> args_;
    };
    const char* input_file_;
    const char* output_file_;
    std::vector<FilterInfo> filters_;

    ArgParser(int argc, char** argv);
};