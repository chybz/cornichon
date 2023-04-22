#pragma once

#include <vector>

#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/cucumber/model/dataset.hpp>
#include <cornichon/cucumber/model/step.hpp>
#include <cornichon/types.hpp>

namespace cornichon::cucumber::model {

struct scenario
{
    std::string name;
    model::line description;
    model::steps steps;
    model::datasets datasets;
    bool background = false;
    std::string keyword;
    std::string keyword_original;
    model::line line;
    strings tags;
};

using scenarios = std::vector<scenario>;

}
