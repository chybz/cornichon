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
    std::string content;

    bool is_blank() const;
    bool is_comment() const;

    void strip();
};

using lines = std::vector<line>;

}
