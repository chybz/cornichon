#pragma once

#include <string>
#include <vector>

#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/cucumber/model/document.hpp>
#include <cornichon/cucumber/model/scenario.hpp>

#include <cornichon/types.hpp>

namespace cornichon::cucumber::model {

struct feature
{
    std::string name;
    line name_line;
    lines satisfaction;
    model::document document;
    model::scenario background;
    std::string keyword_original;
    model::scenarios scenarios;
    strings tags;
    std::string language = "en";
};

using features = std::vector<feature>;

}
