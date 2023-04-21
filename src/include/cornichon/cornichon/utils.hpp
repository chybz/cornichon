#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <chrono>
#include <typeinfo>
#include <type_traits>
#include <functional>
#include <utility>
#include <filesystem>

#include <cornichon/types.hpp>
#include <cornichon/join_utils.hpp>
#include <cornichon/indent_utils.hpp>

namespace cornichon {

bool interactive();

void set_progress(std::size_t x, std::size_t n, std::size_t w = 50);
void clear_progress(std::size_t w = 50);

std::string home_directory();
std::string exe_path();
bool exists(const std::string_view& path);
bool directory_exists(const std::string_view& path);
bool file_exists(const std::string_view& path);
bool touch_file(const std::string& path);
bool truncate_file(const std::string& path, std::size_t size);
bool copy_file(const std::string& from, const std::string& to);
bool write_file(const std::string& file, const char* buffer, std::size_t size);
std::size_t file_size(const std::string_view& path);
std::size_t file_size_if_exists(const std::string_view& path);
std::size_t file_mtime(const std::string& path);
bool extension_is(const std::string_view& path, const std::string_view& ext);
bool file_extension_is(const std::string_view& path, const std::string_view& ext);
std::string slurp(const std::string& path);

bool mkpath(const std::string_view& path);
std::size_t rmpath(const std::string_view& path);
bool rmfile(const std::string_view& path);
bool mkfilepath(const std::string_view& path);
void rename(const std::string_view& from_path, const std::string_view& to_path);
std::string fullpath(const std::string_view& rel);
std::string fullpath(const std::string_view& dir, const std::string_view& file);
std::string basename(const std::string_view& path, const std::string_view& ext = {});
std::string dirname(const std::string_view& path);

std::string
quote_meta(const std::string_view& unquoted);

strings
find_files(const std::string_view& path, const std::string_view& re);

std::size_t human_readable_size(const std::string& expr);

std::string human_readable_size(
    std::size_t size,
    const std::string& user_unit = "B"
);

std::string human_readable_file_size(const std::string& file);

std::string human_readable_rate(
    std::size_t count,
    std::size_t millisecs,
    const std::string& user_unit = "B"
);

std::string
plural(
    const std::string& base,
    const std::string& p,
    std::size_t count
);

std::string
toupper(const std::string& s);

bool has_spaces(const std::string& s);
bool has_spaces(const char* s);

template <typename What>
bool
contains(const std::string& s, What&& w)
{ return s.find(w) != std::string::npos; }

void
split(
    const std::string_view& re,
    const std::string_view& expr,
    string_views& list
);

string_views split_lines(const std::string_view& text);

std::string
subst(const std::string& s, const std::string& re, const std::string& what);

void chomp(std::string& s);

strings glob(const std::string& expr);

bool match(const std::string& expr, const std::string& re);
bool imatch(const std::string& expr, const std::string& re);

bool
extract_delimited(
    std::string& from,
    std::string& to,
    unsigned char delim = ';',
    unsigned char quote = '\0'
);

std::string demangle_type_name(const std::string& mangled);

std::string escape(const char c);
std::string escape_for_string(const char c);
std::string escape(const std::string& s);

ssize_t send_file(int out_fd, int in_fd, off_t* offset, size_t count);

std::string
find_cmd(
    const std::string& cmd,
    const std::string& env_var = {}
);

std::string
try_find_cmd(
    const std::string& cmd,
    const std::string& env_var = {}
);

struct in_directory
{
    in_directory(const std::string& dir)
    : prev_dir(std::filesystem::current_path())
    { std::filesystem::current_path(dir); }

    ~in_directory()
    { std::filesystem::current_path(prev_dir); }

    std::string prev_dir;
};

template <typename Callable>
void
call_in_directory(const std::string& dir, Callable&& cb)
{
    in_directory d(dir);
    cb();
}

template <typename T>
inline
std::string
type_info(const T& t)
{ return demangle_type_name(typeid(t).name()); }

template <typename T>
inline
std::string
type_info()
{ return demangle_type_name(typeid(T).name()); }

inline
string_views
split(const std::string_view& re, const std::string_view& expr)
{
    string_views list;

    split(re, expr, list);

    return list;
}

struct indent_info
{
    std::string pad = "    ";
    std::string sep = "\n";
};

template <typename... Args>
std::string
indent_with(const indent_info& ii, Args&&... args)
{
    std::ostringstream oss;
    auto sep_pad = ii.sep + ii.pad;

    std::size_t prev_count = 0;

    auto indent_arg = [&](auto&& arg) {
        using type = std::decay_t<decltype(arg)>;

        if (prev_count) {
            oss << ii.sep;
        }

        oss << ii.pad;

        if constexpr (is_indentable_container_v<type>) {
            oss << join(sep_pad, arg);
        } else {
            oss << join(sep_pad, split(ii.sep, arg));
        }

        ++prev_count;
    };

    (indent_arg(std::forward<Args>(args)), ...);

    return oss.str();
}

template <typename... Args>
std::string
indent_with(const std::string& pad, Args&&... args)
{
    return
        indent_with(
            indent_info{ pad },
            std::forward<Args>(args)...
        );
}

template <typename... Args>
std::string
indent(Args&&... args)
{ return indent_with(indent_info{}, std::forward<Args>(args)...); }

template <typename T>
class reverse
{
public:
  explicit reverse(T& iterable)
  : iterable_{iterable}
  {}

  auto begin() const
  { return std::rbegin(iterable_); }

  auto end() const
  { return std::rend(iterable_); }

private:
  T& iterable_;
};

template <typename... Args>
std::string
cat(Args&&... args)
{ return join("", std::forward<Args>(args)...); }

template <typename... Args>
std::string
cat_dir(Args&&... args)
{ return join("/", std::forward<Args>(args)...); }

template <typename... Args>
std::string
cat_file(Args&&... args)
{ return cat_dir(std::forward<Args>(args)...); }

template <typename T>
inline
T to_num(const std::string& s)
{ return ((T) std::strtoull(s.c_str(), NULL, 10)); }

}
