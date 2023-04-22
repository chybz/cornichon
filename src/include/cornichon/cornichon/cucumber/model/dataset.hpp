#pragma once

#include <string>
#include <vector>

#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/types.hpp>

namespace cornichon::cucumber::model {

struct dataset
{
    std::string name;
    model::line description;
    // TODO data -- csv2 ??
    model::line line;
    strings tags;
};

using datasets = std::vector<dataset>;

}
