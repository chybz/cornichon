#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace cornichon::cucumber::model {

struct line
{
    std::size_t number;
    std::string document;
    std::string raw_content;

    bool is_blank() const;
    bool is_comment() const;
};

using lines = std::vector<line>;

}
