#include <regex>

#include <cornichon/cucumber/model/line.hpp>

namespace cornichon::cucumber::model {

static const std::regex blank_re("\\S*");
static const std::regex comment_re("\\S*#.*");

bool
line::is_blank() const
{ return std::regex_match(raw_content, blank_re); }

bool
line::is_comment() const
{ return std::regex_match(raw_content, comment_re); }

}
