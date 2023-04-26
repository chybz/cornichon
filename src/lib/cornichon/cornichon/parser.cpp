#include <cornichon/parser.hpp>

namespace cornichon {

parser::parser(
    std::string lang,
    parser_variant pv
)
: lang_(lang),
pv_(pv)
{}

parser::~parser()
{}

void
parser::reset()
{ pd_ = {}; }

}
