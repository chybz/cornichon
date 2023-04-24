#include <cornichon/cucumber/model/document.hpp>
#include <cornichon/utils.hpp>

namespace cornichon::cucumber::model {

document::document()
{}

document::document(const model::file& f, const std::string& content)
: file(f)
{
    for (std::size_t counter = 0; const auto& lv : split_lines(content)) {
        line l{
            .number = ++counter,
            .document = file.name,
            .raw_content{ lv.begin(), lv.end() }
        };

        l.strip();
        lines.emplace_back(std::move(l));
    }
}

}
