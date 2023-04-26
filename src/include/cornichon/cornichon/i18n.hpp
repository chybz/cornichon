#pragma once

#include <cornichon/types.hpp>

namespace cornichon {

using keywords_map = std::unordered_map<std::string_view, string_views>;
using keywords_maps = std::unordered_map<std::string_view, keywords_map>;

const keywords_map&
keywords(const std::string_view& language);

const string_views&
keywords(const std::string_view& language, const std::string_view& kw);

}
