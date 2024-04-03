//
// created by kacchan on 2024/3/27
// 

#include <iostream>

#include "parser/arg_parser.h"
#include "objects/Image/image.h"
#include "filters/filter_apply.cpp"
#include "reader/reader.h"
#include "writer/writer.h"

int main(int argc, char** argv) {
    try {
        ArgParser parsed_args(argc, argv);
        const char* filename = parsed_args.input_file_;
        Reader reader(filename);
        Image image = reader.Read();
        std::vector<ArgParser::FilterInfo> filters = parsed_args.filters_;
        ApplyFilters(filters, image);
        Writer::WriteFile(parsed_args.output_file_, image);
        std::cout << "Image saved as " << parsed_args.output_file_ << std::endl;
    } catch (std::exception& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }
}