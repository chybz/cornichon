#pragma once

#include <regex>
#include <charconv>

#include <cornichon/utils.hpp>
#include <cornichon/types.hpp>
#include <cornichon/log.hpp>

namespace cornichon {

struct regex_result
{
    bool match = false;
    string_views matches;
};

namespace detail {

struct null_arg{};

template <typename SubMatch, typename Arg = null_arg>
std::string_view
extract_submatch(const SubMatch& sm, Arg&& a)
{
    std::string_view sv{sm.first, sm.second};

    if constexpr (
        std::is_same_v<Arg, std::string>
        ||
        std::is_same_v<Arg, std::string_view>
    ) {
        a.assign(sv);
    } else if constexpr (
        std::is_integral_v<Arg>
        ||
        std::is_floating_point_v<Arg>
    ) {
        auto [p, ec] = std::from_chars(sv.begin(), sv.end(), a);

        die_unless(
            ec == std::errc(),
            "failed to convert \"",
            sv,
            "\" to ",
            type_info(a)
        );
    } else if constexpr (!std::is_same_v<Arg, null_arg>) {
        die("unsupported argument: ", type_info(a));
    }

    return sv;
}

template <std::size_t N, typename MatchResult>
void
check_match_args(MatchResult&& m)
{
    if constexpr (N > 0) {
        auto expected = m.size() - 1;

        die_unless(
            N == expected,
            "incorrect match args: ",
            "expected ", expected,
            ", got ", N
        );
    }
}

template <typename MatchResult, typename... Args>
void
extract_submatches(MatchResult&& m, Args&&... args)
{
    constexpr auto nargs = sizeof...(args);

    check_match_args<nargs>(m);

    auto mit = m.begin();

    if constexpr (nargs > 0) {
        (extract_submatch(*++mit, std::forward<Args>(args)), ...);
    }
}

template <typename MatchResult>
void
extract_submatches(MatchResult&& m, string_views& vs)
{
    auto mit = m.begin();

    while (++mit != m.end()) {
        vs.push_back(std::string_view{mit->first, mit->second});
    }
}

} // namespace detail

template <typename... Args>
bool
full_match(
    const std::string_view& e,
    const std::regex& re,
    Args&&... args
)
{
    std::cmatch m;

    bool match = std::regex_match(e.begin(), e.end(), m, re);

    if (match) {
        detail::extract_submatches(m, std::forward<Args>(args)...);
    }

    return match;
}

template <typename... Args>
bool
full_match(
    const std::string_view& e,
    const std::string_view& pat,
    Args&&... args
)
{
    std::regex re(pat.data(), pat.size());

    return full_match(e, re, std::forward<Args>(args)...);
}

template <typename... Args>
bool
partial_match(
    const std::string_view& e,
    const std::string_view& pat,
    Args&&... args
)
{
    std::cmatch m;
    std::regex re(pat.data(), pat.size());

    bool match = std::regex_search(e.begin(), e.end(), m, re);

    if (match) {
        detail::extract_submatches(m, std::forward<Args>(args)...);
    }

    return match;
}

}
