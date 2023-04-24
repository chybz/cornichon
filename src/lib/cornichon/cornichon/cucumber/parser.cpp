#include <unordered_map>
#include <regex>

#include <cornichon/cucumber/parser.hpp>

#include <cornichon/utils.hpp>
#include <cornichon/log.hpp>
#include <cornichon/regex.hpp>

namespace cornichon::cucumber {

using matcher_map = std::unordered_map<std::string_view, std::regex>;

static const std::string_view step_line_first = "step_line_first";
static const std::string_view step_line_cont = "step_line_cont";
static const std::string_view feature_line = "feature_line";
static const std::string_view scenario_line = "scenario_line";
static const std::string_view examples_line = "examples_line";
static const std::string_view table_line = "table_line";
static const std::string_view tags_line = "tags_line";

const std::regex&
get_matcher(const std::string_view& k)
{
    static const matcher_map mm = {
        {
            step_line_first,
            std::regex{"^(given|and|when|then|but)(.+)"}
        },
        {
            step_line_cont,
            std::regex{"^(given|when|then)(.+)"}
        },
        {
            feature_line,
            std::regex{"^(feature): (.+)"}
        },
        {
            scenario_line,
            std::regex{"^(background|scenario|scenarioOutline): ?(.+)?"}
        },
        {
            examples_line,
            std::regex{"^(examples): ?(.+)?$"}
        },
        {
            table_line,
            std::regex{"^\\s*\\|"}
        },
        {
            tags_line,
            std::regex{"@(^\\s]+)"}
        }
    };

    return mm.at(k);
}

template <typename... Args>
bool
matches(
    const std::string& line,
    const std::string_view& k,
    Args&&... args
)
{ return full_match(line, get_matcher(k), std::forward<Args>(args)...); }

bool
is_step_line(bool continuation, const std::string& line)
{ return matches(line, continuation ? step_line_cont : step_line_first); }

parser::parser()
{}

parser::~parser()
{}

model::feature
parser::parse(const model::file& file) const
{
    auto content = slurp(file.name);

    return model::feature{ .document{file, content} };
}

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
    std::string_view kw;
    std::string_view name;

    while (it != lines.end()) {
        const auto& line = *it++;

        if (line.is_comment()) {
            continue;
        }

        if (line.is_blank()) {
            break;
        }

        if (matches(line.content, feature_line, kw, name)) {
            f.name = name;
            f.keyword_original = kw;
            f.name_line = line;
            f.tags = feature_tags;

            break;
        } else if (partial_match(line.content, "@([^\\s]+)", feature_tags)) {
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

    while (it != lines.end()) {
        const auto& line = *it++;

        if (line.is_comment() || line.is_blank()) {
            continue;
        }

        if (
            matches(line.content, scenario_line)
            ||
            matches(line.content, tags_line)
        ) {
            --it;
            break;
        } else {
            f.satisfaction.push_back(line);
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
            last_scenario.keyword_original == "Scenario Outline"
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

model::lines
parser::extract_scenarios(
    model::feature& f,
    const model::lines& from
) const
{
    std::size_t scenarios = 0;
    auto lines = from;
    auto it = lines.begin();
    strings tags;
    std::string_view type;
    std::string_view name;

    while (it != lines.end()) {
        const auto& line = *it++;

        if (line.is_comment() || line.is_blank()) {
            continue;
        }

        if (matches(line.content, examples_line, type, name)) {
            die_if(
                f.scenarios.empty(),
                "'Examples:' line before scenario definition"
            );

            auto& last_scenario = f.scenarios.back();

            model::dataset ds{
                .name{name.data(), name.size()},
                .line = line
            };

            if (!tags.empty()) {
                ds.tags = last_scenario.tags;
                ds.tags.insert(ds.tags.end(), tags.begin(), tags.end());
            } else {
                ds.tags = tags;
            }

            tags.clear();
            lines = extract_examples_description(ds, lines);
            lines = extract_table(ds, remove_next_blanks(lines));
            it = lines.begin();

            if (!last_scenario.datasets.empty()) {
                const auto& prev_ds = last_scenario.datasets[0];
                // TODO: check consistent data
            }

            last_scenario.datasets.emplace_back(std::move(ds));
        } else if (matches(line.content, scenario_line, type, name)) {
            finish_scenario(f, line);

            //if (scenarios++ && )
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

model::lines
parser::extract_examples_description(
    model::dataset& d,
    const model::lines& from
) const
{
    auto lines = from;
    auto it = lines.begin();

    while (it != lines.end()) {
        const auto& line = *it++;

        if (line.is_comment()) {
            continue;
        }

        if (
            matches(line.content, table_line)
            || matches(line.content, examples_line)
            || matches(line.content, tags_line)
            || matches(line.content, scenario_line)
        ) {
            --it;
            break;
        }

        d.description.push_back(line);
    }

    lines.erase(lines.begin(), it);

    return lines;
}

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

model::lines
parser::extract_table(
    model::dataset& d,
    const model::lines& from
) const
{
    auto lines = from;
    auto it = lines.begin();

    while (it != lines.end()) {
        const auto& line = *it++;

        if (line.is_comment()) {
            continue;
        }

        if (!line.content.starts_with('|')) {
            --it;
        }
    }

    lines.erase(lines.begin(), it);

    return lines;
}

}
