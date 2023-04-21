#pragma once

#include <string>

#include <cornichon/cucumber/model/file.hpp>
#include <cornichon/cucumber/model/line.hpp>

namespace cornichon::cucumber::model {

struct document
{
    document();
    document(const model::file& f, const std::string& content);

    model::file file;
    std::string content;
    model::lines lines;
};

}
