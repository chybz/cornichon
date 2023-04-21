#pragma once

#include <type_traits>

#include <cornichon/container_helpers.hpp>

namespace cornichon {

template <typename T>
struct is_indentable_container
{
    using type = std::conditional_t<
        is_vector_v<T>
        || is_set_v<T>
        || is_unordered_set_v<T>,
        std::true_type,
        std::false_type
    >;

    static constexpr type::value_type value = type::value;
};

template <typename T>
using is_indentable_container_t = typename is_indentable_container<T>::type;

template <typename T>
constexpr bool is_indentable_container_v = is_indentable_container<T>::value;

}
