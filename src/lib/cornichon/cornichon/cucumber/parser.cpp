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

match_result
parser::is_step_line(bool continuation, const std::string& line) const
{
    return
        extract_matches(
            line,
            continuation ? "step_line_cont" : "step_line_first"
        );
}

match_result
parser::is_feature_line(const std::string& line) const
{ return extract_matches(line, "feature_line"); }

match_result
parser::is_scenario_line(const std::string& line) const
{ return extract_matches(line, "scenario_line"); }

match_result
parser::is_table_line(const std::string& line) const
{ return extract_matches(line, "table_line"); }

match_result
parser::is_tags_line(const std::string& line) const
{ return extract_matches(line, "tags_line"); }

match_result
parser::is_examples_line(const std::string& line) const
{ return extract_matches(line, "examples_line"); }

model::lines
parser::remove_next_blanks(const model::lines& from) const
{
    model::lines to;

    std::copy_if(
        from.begin(), from.end(),
        std::back_inserter(to),
        [](const auto& line) { return !line.is_blank(); }
    );

    return to;
}

model::lines
parser::extract_feature_name(
    model::feature& f,
    const model::lines& from
) const
{
    auto lines = from;

    for (auto it = lines.begin(); it != lines.end(); ) {
        const auto& line = *it;

        if (line.is_comment()) {
            continue;
        }

        if (line.is_blank()) {
            break;
        }


    }
}

void
parser::extract_conditions_of_satisfaction(
    model::feature& f,
    model::lines& lines
) const
{}

void
parser::finish_scenario(
    model::feature& f,
    model::line& line
) const
{}

void
parser::extract_scenarios(
    model::feature& f,
    model::lines& lines
) const
{}

void
parser::extract_steps(
    model::feature& f,
    model::scenario& s,
    model::lines& lines
) const
{}

/*void
parser::extract_examples_description(
    model::examples& es,
    model::lines& lines
) const
{}*/

void
parser::extract_scenario_description(
    model::scenario& s,
    model::lines& lines
) const
{}

void
parser::extract_multiline_string(
    model::feature& f,
    model::scenario& s,
    model::step& st,
    model::lines& lines
) const
{}

void
parser::extract_table(
    model::feature& f,
    model::scenario& s,
    model::step& st,
    model::lines& lines
) const
{}

bool
parser::matches(const std::string& line, const std::string& key) const
{ return std::regex_match(line, matchers_.at(key)); }

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
