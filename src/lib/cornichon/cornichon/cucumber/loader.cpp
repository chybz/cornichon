#include <cornichon/cucumber/loader.hpp>
#include <cornichon/cucumber/parser.hpp>
#include <cornichon/utils.hpp>

namespace cornichon::cucumber {

model::features
loader::load(const std::string& path) const
{
    parser p;
    model::features fs;

    for (auto& fname : find_files(path, ".*\\.feature")) {
        auto f = p.parse({ .name = fname });

        fs.emplace_back(std::move(f));
    }

    return fs;
}

}
