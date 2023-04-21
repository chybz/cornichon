#pragma once

#include <string>
#include <vector>

#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/cucumber/model/document.hpp>

namespace cornichon::cucumber::model {

struct feature
{
    std::string name;
    line name_line;
    lines satisfaction;
    model::document document;
};

using features = std::vector<feature>;

}
