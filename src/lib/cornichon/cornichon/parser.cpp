#include <cornichon/parser.hpp>
#include <cornichon/utils.hpp>

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

void
parser::parse(const std::string& filename)
{
    reset();
    pd_.filename = filename;

    for (const auto& l : split_lines(slurp(pd_.filename))) {
        pd_.line++;
        l = strip(l);

        if (l.empty() && pd_.state != parser_state::multiline_text) {
            continue;
        }

        action(line);
    }

    if (pd_.table()) {
        action_table("");
    }

    auto feature = pd_.feature;

    reset();

    return feature;
}

}
