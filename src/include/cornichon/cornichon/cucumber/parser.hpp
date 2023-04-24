#pragma once

#include <string>

#include <cornichon/cucumber/model/file.hpp>
#include <cornichon/cucumber/model/document.hpp>
#include <cornichon/cucumber/model/feature.hpp>
#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/cucumber/model/scenario.hpp>
#include <cornichon/cucumber/model/step.hpp>

#include <cornichon/utils.hpp>
#include <cornichon/regex.hpp>

namespace cornichon::cucumber {

class parser
{
public:
    parser();
    virtual ~parser();

    model::feature parse(const model::file& file) const;

private:
    model::lines remove_next_blanks(const model::lines& from) const;

    model::lines extract_feature_name(
        model::feature& f,
        const model::lines& from
    ) const;

    model::lines extract_conditions_of_satisfaction(
        model::feature& f,
        const model::lines& from
    ) const;

    void finish_scenario(
        model::feature& f,
        const model::line& line
    ) const;

    model::lines extract_scenarios(
        model::feature& f,
        const model::lines& from
    ) const;

    void extract_steps(
        model::feature& f,
        model::scenario& s,
        model::lines& lines
    ) const;

    model::lines extract_examples_description(
        model::dataset& d,
        const model::lines& from
    ) const;

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

    model::lines extract_table(
        model::dataset& d,
        const model::lines& from
    ) const;
};

}
