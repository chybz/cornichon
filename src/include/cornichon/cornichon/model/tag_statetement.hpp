#pragma once

#include <cornichon/types.hpp>

namespace cornichon::model {

struct tag_statement : basic_statement
{
    std::string filename;
    std::size_t line = 0;
    std::string keyword;
    std::string name;
    std::string captured;
};

}
