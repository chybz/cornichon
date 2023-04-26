#pragma once

#include <cornichon/types.hpp>
#include <cornichon/i18n.hpp>
#include <cornichon/model/table.hpp>

namespace cornichon {

enum class parser_variant
{
    feature
};

enum class parser_state
{
    init,
    multiline_text
};

struct parser_data
{
    parser_state ps_ = parser_state::init;
    std::size_t line = 0;
    std::string multiline_start;
    std::string multiline_leading;
    std::string multiline_terminator;
    std::string filename;
    std::string scenario_container;
    std::string feature;
    std::string rule;
    std::string parent;
    std::string statement;
    strings tags;
    strings lines;
    model::table table;
    strings examples;
};

class parser
{
public:
    parser(
        std::string lang = "en",
        parser_variant pv = parser_variant::feature
    );

    virtual ~parser();

    void reset();

    void parse(const std::string& filename);

private:
    std::string lang_;
    parser_variant pv_;
    parser_data pd_;
    string_map keywords_;
};

}
