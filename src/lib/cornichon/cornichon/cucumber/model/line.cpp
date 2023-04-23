#include <regex>

#include <cornichon/cucumber/model/line.hpp>
#include <cornichon/utils.hpp>

namespace cornichon::cucumber::model {

static const std::regex blank_re("\\S*");
static const std::regex comment_re("\\S*#.*");

bool
line::is_blank() const
{ return std::regex_match(raw_content, blank_re); }

bool
line::is_comment() const
{ return std::regex_match(raw_content, comment_re); }

std::string
line::content() const
{
    auto s = subst(raw_content, "^\\s+", "");
    s = subst(s, "\\s+$", "");

    return s;
}

}
