#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/types.hpp>

namespace cornichon::cucumber::model {

using table_row = std::unordered_map<std::string, std::string>;
using table = std::vector<table_row>;

struct dataset
{
    std::string name;
    model::lines description;
    table data;
    model::line line;
    strings tags;
};

using datasets = std::vector<dataset>;

}
