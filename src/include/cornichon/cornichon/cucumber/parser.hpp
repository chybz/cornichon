#pragma once

#include <string>

#include <cornichon/cucumber/model/file.hpp>
#include <cornichon/cucumber/model/document.hpp>
#include <cornichon/cucumber/model/feature.hpp>

namespace cornichon::cucumber {

class parser
{
public:
    parser();
    virtual ~parser();

    model::feature parse(const model::file& file) const;

private:
};

}
