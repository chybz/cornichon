#pragma once

#include <string>

namespace cornichon::model {

struct basic_statement
{
    std::string filename;
    std::size_t line = 0;
    std::string keyword;
    std::string name;
    std::string captured;
};

}
