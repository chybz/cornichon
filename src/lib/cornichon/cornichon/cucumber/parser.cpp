#include <cornichon/cucumber/parser.hpp>

#include <cornichon/utils.hpp>
#include <cornichon/log.hpp>

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
    auto it = lines.begin();
    strings feature_tags;

    for (; it != lines.end(); ++it) {
        const auto& line = *it;

        if (line.is_comment()) {
            continue;
        }

        if (line.is_blank()) {
            break;
        }

        m = is_feature_line(line.content());

        if (m) {
            f.name = m[1];
            f.keyword_original = m[0];
            f.name_line = line;
            f.tags = feature_tags;

            break;
        } else if (m = extract_matches(line.content(), "@([^\\s]+)")) {
            feature_tags.insert(
                feature_tags.end(),
                std::make_move_iterator(m.matches.begin()),
                std::make_move_iterator(m.matches.end())
            );
        } else {
            // TODO: improve this later
            die(
                "malformed feature line (expecting: /^(feature): (.+)/",
                line.raw_content
            );
        }
    }

    lines.erase(lines.begin(), it);

    return remove_next_blanks(lines);
}

model::lines
parser::extract_conditions_of_satisfaction(
    model::feature& f,
    const model::lines& from
) const
{
    auto lines = from;
    auto it = lines.begin();

    for (; it != lines.end(); ++it) {
        const auto& line = *it;

        if (line.is_comment() || line.is_blank()) {
            continue;
        }

        if (
            is_scenario_line(line.content())
            ||
            is_tags_line(line.content())
        ) {
            --it;
            break;
        } else {
            f.satisfactions.push_back(line);
        }
    }

    lines.erase(lines.begin(), it);

    return remove_next_blanks(lines);
}

void
parser::finish_scenario(
    model::feature& f,
    const model::line& line
) const
{
    if (!f.scenarios.empty()) {
        const auto& last_scenario = f.scenarios.back();

        if (
            last_scenario.keyword_original = "Scenario Outline"
            &&
            last_scenario.datasets.empty()
        ) {
            die(
                "Outline scenario expects 'Examples:' section",
                line.raw_content
            );
        }
    }
}

model::lines lines
parser::extract_scenarios(
    model::feature& f,
    const model::lines& from
) const
{
    std::size_t scenarios = 0;
    auto lines = from;
    auto it = lines.begin();
    strings tags;

    for (; it != lines.end(); ++it) {
        const auto& line = *it;

        if (line.is_comment() || line.is_blank()) {
            continue;
        }

        if (auto m = is_examples_line(line.content())) {
            die_if(
                f.scenarios.emtpy(),
                "'Examples:' line before scenario definition"
            );

            auto& last_scenario = f.scenarios().back();
            model::dataset ds{.name = m[1], .line = line};

            if (!tags.empty()) {
                ds.tags = last_scenario.tags;
                std::insert(ds.tags.end(), tags.begin(), tags.end());
            } else {
                ds.tags = tags;
            }

            tags.clear();
            lines = extract_examples_description(ds, lines);
            lines = extract_table(6, ds, remove_next_blanks(lines));
            it = lines.begin();

            if (!last_scenario.datasets.empty()) {
                const auto& prev_ds = last_scenario.datasets[0];
                // TODO: check consistent data
            }

            last_scenario.datasets.emplace_back(std::move(ds));
        } else if (auto m = is_scenario_line(line.content())) {
            finish_scenario(f, line);

            if (scenarios++ && )
        }
    }

    lines.erase(lines.begin(), it);

    return remove_next_blanks(lines);
}

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
