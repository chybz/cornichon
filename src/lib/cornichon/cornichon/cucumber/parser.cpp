#include <cornichon/cucumber/parser.hpp>

#include <cornichon/utils.hpp>

namespace cornichon::cucumber {

parser::parser()
{
    make_matchers();
}

parser::~parser()
{}

model::feature
parser::parse(const model::file& file) const
{
    auto content = slurp(file.name);

    return model::feature{ .document{file, content} };
}

void
parser::make_matchers()
{
    matchers_["step_line_first"] =
        "^(" + join("|", "given", "and", "when", "then", "but") + ")"
        + "(.+)"
        ;
    matchers_["step_line_cont"] =
        "^(" + join("|", "given", "when", "then") + ")"
        + "(.+)"
        ;
    matchers_["feature_line"] = "^(feature): (.+)";
    matchers_["scenario_line"] =
        "^(" + join("|", "background", "scenario", "scenarioOutline") + ")"
        + ": ?(.+)?"
        ;
    matchers_["example_line"] = "^(examples): ?(.+)?$";
    matchers_["table_line"] = "^\\s*\\|";
    matchers_["tags_line"] = "@(^\\s]+)";
}

}
