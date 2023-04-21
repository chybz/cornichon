#pragma once

#include <cornichon/cucumber/model/feature.hpp>

namespace cornichon::cucumber {

struct loader
{
    model::features load(const std::string& path) const;
};

}
