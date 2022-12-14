#include <iostream>
#include <fstream>
#include <sstream>
#include "clipper2/clipper.h"
#include "clipper2/clipper.offset.h"

#define CLIPPER_PRECISION 6
#define INFLATION_DELTA 1.0e-5

using Clipper2Lib::PathD;
using Clipper2Lib::PathsD;
using Clipper2Lib::PointD;
using Clipper2Lib::FillRule;
using Clipper2Lib::JoinType;
using Clipper2Lib::EndType;
using Clipper2Lib::Union;

void dumpPath(const PathD &path) {
    std::cout << "[";

    for (int index = 0; index < path.size(); index += 1) {
        if (index > 0) {
            std::cout << ",";
        }
        std::cout
            << std::fixed
            << std::setprecision(CLIPPER_PRECISION)
            << "(" << path[index].x << "," << path[index].y << ")";
    }
    std::cout << "]" << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Path to polygon list required" << std::endl;
        return -1;
    }

    auto subjects = PathsD {};
    std::string filename { argv[1] };
    std::ifstream polys_file { filename };
    std::string line;

    if (!polys_file.is_open()) {
        std::cerr << "Not open." << std::endl;
        return -1;
    }

    while (std::getline(polys_file, line)) {
        auto current_path = PathD {};
        std::istringstream line_buffer {line };
        std::deque<double> line_values {
            std::istream_iterator<double> { line_buffer },
            std::istream_iterator<double>()
        };

        while (!line_values.empty()) {
            auto x = line_values.front();

            line_values.pop_front();

            if (!line_values.empty()) {
                auto y = line_values.front();

                line_values.pop_front();

                current_path.push_back(PointD { x, y });
            }
        }

        auto to_inflate = PathsD {};

        to_inflate.push_back(current_path);

        auto inflated = Clipper2Lib::InflatePaths(to_inflate, INFLATION_DELTA, JoinType::Square, EndType::Polygon, 0.01, CLIPPER_PRECISION);

        if (inflated.size() != 1) {
            std::cerr << "Expected exactly one result from inflation" << std::endl;
        } else {
            subjects.push_back(inflated.front());
        }
    }

    for (auto path : subjects) {
        dumpPath(path);
    }

    auto merged = Union(subjects, PathsD {}, FillRule::Positive, CLIPPER_PRECISION);
    std::cout << "Merged with " << merged.size() << " outputs" << std::endl;
    for (auto path : merged) {
        dumpPath(path);
    }

    return 0;
}
