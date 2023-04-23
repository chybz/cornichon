#pragma once

#include <string>
#include <unordered_map>
#include <regex>

#include <cornichon/cucumber/model/file.hpp>
#include <cornichon/cucumber/model/document.hpp>
#include <cornichon/cucumber/model/feature.hpp>
#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/cucumber/model/scenario.hpp>
#include <cornichon/cucumber/model/step.hpp>

namespace cornichon::cucumber {

class parser
{
public:
    parser();
    virtual ~parser();

    model::feature parse(const model::file& file) const;

private:
    using matcher_map = std::unordered_map<std::string, std::regex>;

    bool is_step_line(bool continuation, const std::string& line) const;
    bool is_feature_line(const std::string& line) const;
    bool is_scenario_line(const std::string& line) const;
    bool is_table_line(const std::string& line) const;
    bool is_tags_line(const std::string& line) const;
    bool is_examples_line(const std::string& line) const;

    model::lines remove_next_blanks(const model::lines& from) const;

    void extract_feature_name(
        model::feature& f,
        model::lines& lines
    ) const;

    void extract_conditions_of_satisfaction(
        model::feature& f,
        model::lines& lines
    ) const;

    void finish_scenario(
        model::feature& f,
        model::line& line
    ) const;

    void extract_scenarios(
        model::feature& f,
        model::lines& lines
    ) const;

    void extract_steps(
        model::feature& f,
        model::scenario& s,
        model::lines& lines
    ) const;

    /*void extract_examples_description(
        model::examples& es,
        model::lines& lines
    ) const;*/

    void extract_scenario_description(
        model::scenario& s,
        model::lines& lines
    ) const;

    void extract_multiline_string(
        model::feature& f,
        model::scenario& s,
        model::step& st,
        model::lines& lines
    ) const;

    void extract_table(
        model::feature& f,
        model::scenario& s,
        model::step& st,
        model::lines& lines
    ) const;

    bool matches(const std::string& line, const std::string& key) const;
    void make_matchers();

    matcher_map matchers_;
};

}
