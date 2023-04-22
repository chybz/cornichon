#pragma once

#include <string>
#include <unordered_map>
#include <regex>

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
    using matcher_map = std::unordered_map<std::string, std::regex>;

    bool is_step_line(bool continuation, const std::string& line) const;
    bool is_feature_line(const std::string& line) const;
    bool is_scenario_line(const std::string& line) const;
    bool is_table_line(const std::string& line) const;
    bool is_tags_line(const std::string& line) const;
    bool is_examples_line(const std::string& line) const;

    void remove_next_blanks(model::lines& lines) const;

    void extract_feature_name(
        model::feature& f,
        model::lines& lines
    ) const;

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

    void make_matchers();

    matcher_map matchers_;
};

}
