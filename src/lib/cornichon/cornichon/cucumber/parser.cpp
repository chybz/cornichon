#include <cornichon/cucumber/parser.hpp>

#include <cornichon/utils.hpp>

namespace cornichon::cucumber {

parser::parser()
{}

parser::~parser()
{}

model::feature
parser::parse(const model::file& file) const
{
    auto content = slurp(file.name);
    model::document d;

    return model::feature{ .document{file, content} };
}

}
