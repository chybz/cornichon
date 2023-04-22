#pragma once

#include <string>
#include <vector>

#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/types.hpp>

namespace cornichon::cucumber::model {

struct step
{
    std::string text;
    std::string verb;
    std::string verb_original;
    model::line line;
    // TODO: data as string or table
    strings data_as_strings;
    strings columns;
};

using steps = std::vector<step>;

}
