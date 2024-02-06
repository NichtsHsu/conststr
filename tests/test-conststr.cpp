#include <array>
#include <functional>
#include <span>

#include "conststr.hpp"

using conststr::cstr;
using conststr::sv;
namespace charutils = conststr::charutils;
using namespace conststr::literal;

int main() {
    constexpr auto hello = "hello"_cs;

    // Compare
    static_assert(hello == "hello"_cs);
    static_assert(hello == "hello");
    static_assert(hello != "HELLO"_cs);
    static_assert(hello != "HELLO");
    static_assert(hello > "hell"_cs);
    static_assert(hello > "hell");
    static_assert(hello < "helloo"_cs);
    static_assert(hello < "helloo");

    // String literal v.s. character literal
    static_assert("a"_cs == 'a'_cs);

    // Change view type
    constexpr auto hello_span = hello.with_view<std::span<const char>>();
    static_assert(std::same_as<typename decltype(hello_span)::view_type,
                               std::span<const char>>);
    static_assert(hello_span == hello);

    // Concatenate
    constexpr auto hello_world = hello + ' ' + "world" + "!"_cs;

    // String operations
    static_assert(hello_world == "hello world!");
    static_assert(hello_world.front() == 'h');
    static_assert(hello_world.back() == '!');
    static_assert(hello_world[5] == ' ');
    static_assert(hello_world ==
                  hello.append(' ').append("world").append("!"_cs));
    static_assert(hello_world == hello.flatten(' ', "world", "!"_cs));
    static_assert(hello_world ==
                  conststr::flatten(hello, ' ', "world", "!"_cs));
    static_assert(hello_world.pop() == "hello world");
    static_assert(hello_world.append<3>('!') == "hello world!!!!");
    static_assert(hello_world.cut<5>() == "hello");
    static_assert(hello_world.insert(5, 'e') == "helloe world!");
    static_assert(hello_world.substr<3, 0>() == "");
    static_assert(hello_world.substr<3, 2>() == "lo");
    static_assert(hello_world.remove_prefix<2>() == "llo world!");
    static_assert(hello_world.remove_suffix<2>() == "hello worl");
    static_assert(hello_world.uppercase() == "HELLO WORLD!");
    static_assert(hello_world.uppercase().lowercase() == hello_world);
    static_assert(hello_world.replace<'o', '0'>() == "hell0 w0rld!");
    static_assert(hello_world.replace(hello_world.cbegin(),
                                      hello_world.cbegin() + 5,
                                      '?') == "????? world!");
    static_assert(hello_world.replace(0, "happy") == "happy world!");
    static_assert(hello_world.erase(4).erase(8) == "hell word!");
    static_assert(hello_world.find('l') == 2);
    static_assert(hello_world.rfind('l') == 9);
    static_assert(hello_world.find('f') == hello_world.npos);
    static_assert(hello_world.find("lo") == 3);
    static_assert(hello_world.find("loo") == hello_world.npos);
    static_assert(hello_world.find_if(charutils::isspace<char>) == 5);
    static_assert(hello_world.find_if(std::not_fn(charutils::isprint<char>)) ==
                  hello_world.npos);
    static_assert(hello_world.contains('w'));
    static_assert(!hello_world.contains('f'));
    static_assert(!hello_world.contains('f'_cs));
    static_assert(hello_world.starts_with('h'));
    static_assert(!hello_world.starts_with('H'));
    static_assert(hello_world.starts_with("hell"));
    static_assert(!hello_world.starts_with("helo"_cs));
    static_assert(hello_world.ends_with('!'));
    static_assert(!hello_world.ends_with('d'));
    static_assert(hello_world.ends_with("d!"_cs));
    static_assert(!hello_world.ends_with("rd!"_cs));

    // Unconventional character type
    // Remember: End the "raw string" with null terminator (0)
    using int_arr = int[];
    static constexpr auto integer_str = cstr(
        int_arr{0x68, 0x65, 0x6c, 0x6c, 0x6f, 0}, sv<std::span<const int>>);
    static_assert(integer_str ==
                  std::array<int, 5>{0x68, 0x65, 0x6c, 0x6c, 0x6f});
    static_assert(integer_str.starts_with(0x68));
    static_assert(integer_str.starts_with(
        cstr(int_arr{0x68, 0x65, 0}, sv<std::span<const int>>)));
    static_assert(integer_str.starts_with(std::array<int, 2>{0x68, 0x65}));
    static_assert(integer_str.ends_with(0x6f));
    static_assert(integer_str.ends_with(
        cstr(int_arr{0x6c, 0x6f, 0}, sv<std::span<const int>>)));
    static_assert(integer_str.ends_with(std::array<int, 2>{0x6c, 0x6f}));
    constexpr auto to_char_str = integer_str.transform(charutils::cast<char>);
    static_assert(to_char_str == "hello");
    static_assert(to_char_str == integer_str.cast<char>());
    static_assert(
        std::same_as<std::remove_cvref_t<decltype(to_char_str)>,
                     std::remove_cvref_t<decltype(integer_str.cast<char>())>>);
    static_assert(
        std::same_as<decltype(to_char_str)::view_type, std::string_view>);
    constexpr auto to_char_str_span =
        integer_str.transform(charutils::cast<char>, 0, integer_str.size(),
                              sv<std::span<const char>>);
    static_assert(to_char_str_span == std::span<const char>("hello", 5));
    static_assert(to_char_str_span ==
                  integer_str.cast<char>(sv<std::span<const char>>));
    static_assert(
        std::same_as<std::remove_cvref_t<decltype(to_char_str_span)>,
                     std::remove_cvref_t<decltype(integer_str.cast<char>(
                         sv<std::span<const char>>))>>);
    static_assert(std::same_as<decltype(to_char_str_span)::view_type,
                               std::span<const char>>);

    std::cout << __FILE__ ": all tests passed." << std::endl;

    return 0;
}