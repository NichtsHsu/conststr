/* MIT License
 *
 * Copyright (c) 2024 Nichts Hsu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*! 
 * @file conststr.hpp
 * @brief Main header file for the constant string type.
 */

/*!
 * @mainpage The conststr libray
 * @brief Operate strings in constant context.
 * @copyright MIT License.
 * 
 * @section usages Basic usages
 * 
 * @subsection create Create constant strings
 * 
 * @code{.cpp}
 * #include "conststr.hpp"
 * 
 * using conststr::cstr;
 * 
 * constexpr auto str = cstr("hello world!");
 * @endcode
 * 
 * or use user-defined string literal (recommended):
 * 
 * @code{.cpp}
 * #include "conststr.hpp"
 * 
 * using conststr::cstr;
 * using namespace conststr::literal;
 * 
 * constexpr auto str = "hello world!"_cs;
 * @endcode
 * 
 * _The above `include`s and `using`s are omitted in subsequent examples._
 * 
 * @subsection print Print strings
 * 
 * The `operator<<(std::ostream &, const cstr &)` has been overloaded, so:
 * 
 * @code{.cpp}
 * constexpr auto str = "hello world!"_cs;
 * std::cout << str << std::endl;
 * @endcode
 * 
 * @subsection compare Compare strings
 * 
 * The `conststr::cstr` instances are compared in lexicographic order:
 * 
 * @code{.cpp}
 * static_assert("hello"_cs > "hell"_cs);
 * @endcode
 * 
 * @subsection concat Concatenate strings
 * 
 * @code{.cpp}
 * constexpr auto hello = "hello"_cs;
 * constexpr auto world = "world"_cs;
 * constexpr auto msg = hello + ' ' + world + '!';
 * // or
 * constexpr auto msg2 = hello.flatten(" "_cs, world, "!"_cs);
 * // or
 * using conststr::flatten;
 * constexpr auto msg3 = flatten(hello, " "_cs, world, "!"_cs);
 * @endcode
 * 
 * @subsection operate Operate strings
 * 
 * @note
 * In order to maintain the uniformity of APIs, all methods that operate on
 * strings do not modify the string itself, but return the modified string.
 * 
 * @code{.cpp}
 * constexpr auto str = "hello world!"_cs;
 * constexpr auto newstr = str.uppercase()    // to "HELLO WORLD!"
 *                             .pop()         // to "HELLO WORLD"
 *                             .erase(4)      // to "HELL WORLD"
 *                             .erase(8)      // to "HELL WORD"
 *                             .append('?');  // to "HELL WORD?"
 * @endcode
 * 
 * Check the document page of `conststr::cstr` to learn more operating methods.
 * 
 * If you really want to modify the original string, `std::transform` and functions
 * in the namespace `conststr::charutils` may be helpful:
 * 
 * @code{.cpp}
 * #include <algorithm>
 * 
 * namespace charutils = conststr::charutils;
 * 
 * constexpr auto fun() {
 *     auto str = "hello world!"_cs;
 *     std::transform(str.begin(), str.end(), str.begin(),
 *                    charutils::replace<'o', 'O'>);  // to "hellO wOrld!"
 *     std::transform(str.begin(), str.begin() + 2, str.begin(),
 *                    charutils::toupper<char>);  // to "HEllO wOrld!"
 *     return str;
 * }
 * @endcode
 * 
 * @subsection view Convert to view
 * 
 * @warning `cstr` can be implicitly converted to its `view` type, but the implicit
 * conversion of a temporary `cstr` r-values may result in dangling pointers.
 * 
 * Whenever you need to obtain string view or internal pointer from a `cstr` r-value,
 * store it to a variable first.
 * 
 * @code{.cpp}
 * constexpr auto str = func_returns_cstr();
 * constexpr view_type sv = str;
 * @endcode
 * 
 * The view type of `cstr` defaults to `std::basic_string_view<T>`. You can choose
 * different view types through the following methods:
 * 
 * @code{.cpp}
 * #include <span>
 * 
 * using conststr::sv;
 * 
 * constexpr auto s1 = cstr("hello world!", sv<std::span<const char>>);
 * constexpr auto s2 = "hello world!"_cs.with_view<std::span<const char>>();
 * @endcode
 * 
 * @section reflect Work with limited reflection
 * 
 * Thanks to the introduction of
 * [structured binding](https://en.cppreference.com/w/cpp/language/structured_binding),
 * we can implement compile-time reflection of aggregate types based on some tricks.
 * 
 * You can include the header file `reflect.hpp` to use `conststr::cstr` with reflection,
 * basically it is a reimplementation of [glaze](https://github.com/stephenberry/glaze)
 * based on `conststr::cstr`.
 * 
 * In order to get reflection you need to use default-constructible aggregate types,
 * Roughly, this means that the type:
 * 
 * - no user-declared or inherited constructors,
 * - no virtual/private/protected base classes,
 * - all members are public,
 * - no virtual member functions,
 * - default-constructible,
 * - no member of reference type (but member of `std::reference_wrapper` with default initializer is allowed).
 * 
 * For example:
 * 
 * @code{.cpp}
 * struct MyStruct {
 *     int number;
 *     double decimal;
 *     std::string name;
 * };
 * @endcode
 * 
 * Then you can get the name of its members via `reflect::name_of`, get the type of
 * its members via `reflect::type_of`/`reflect::type_of_member`, or get the reference
 * to object's member via `reflect::member_of` like the following example:
 * 
 * @code{.cpp}
 * #include "reflect.hpp"
 * 
 * struct MyStruct {
 *     int number;
 *     double decimal;
 *     std::string name;
 * };
 * 
 * int main() {
 *     // Get member type of `MyStruct` via index
 *     reflect::type_of<MyStruct, 0> a = 1;        // type of `a` is `int`
 *     reflect::type_of<MyStruct, 1> b = 1.f;      // type of `b` is `double`
 *     reflect::type_of<MyStruct, 2> c = "hello";  // type of `c` is `std::string`
 * 
 *     // Also you can get member type via its name
 *     reflect::type_of_member<MyStruct, "number"> d = 1;
 *     reflect::type_of_member<MyStruct, "decimal"> e = 1.f;
 *     reflect::type_of_member<MyStruct, "name"> f = "hello";
 * 
 *     // Get member name via index
 *     std::cout << reflect::number_of_members<MyStruct> << std::endl; // 3 members in `MyStruct`
 *     std::cout << reflect::name_of<MyStruct, 0> << std::endl;        // first member is "number"
 *     std::cout << reflect::name_of<MyStruct, 1> << std::endl;        // second member is "decimal"
 *     std::cout << reflect::name_of<MyStruct, 2> << std::endl;        // third member is "name"
 * 
 *     // Get member reference via index ...
 *     MyStruct s;
 *     decltype(auto) numref = reflect::member_of<0>(s);
 *     numref = 100;
 *     std::cout << s.number << std::endl;
 * 
 *     // ...or, via its name
 *     decltype(auto) nameref = reflect::member_of<"name">(s);
 *     nameref += "hello world";
 *     std::cout << s.name << std::endl;
 * 
 *     return 0;
 * }
 * @endcode
 */

#ifndef CONSTSTR_HPP
#define CONSTSTR_HPP

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string_view>
#include <tuple>
#include <type_traits>

/**
 * @brief The outermost namespace of this library to avoid identifier pollution
 */
namespace conststr {

/**
 * @brief Some useful template meta.
 */
namespace meta {
/**
 * @brief Get the first type of class template parameter pack.
 * @details
 * For example, `first_t<T, U, V, W>` is `T`.
 * @tparam Ts the class template parameter pack
 */
template <typename... Ts>
using first_t = std::tuple_element_t<0, std::tuple<Ts...>>;

/**
 * @brief Get the type of the first value of variable template parameter pack.
 * @details
 * For example, `first_t_of<a, b, c, d>` is `decltype(a)`.
 * @tparam Vs the variable template parameter pack
 */
template <auto... Vs>
using first_t_of =
    std::remove_cvref_t<std::tuple_element_t<0, decltype(std::tuple{Vs...})>>;

/**
 * @brief Get the first value of variable template parameter pack.
 * @details
 * For example, `first_v<a, b, c, d>` is `a`.
 * @tparam Vs the variable template parameter pack
 */
template <auto... Vs>
constexpr auto first_v = [](auto first, auto...) { return first; }(Vs...);

/**
 * @brief Get the last type of class template parameter pack.
 * @details
 * For example, `first_t<T, U, V, W>` is `W`.
 * @tparam Ts the class template parameter pack
 */
template <typename... Ts>
using last_t = std::tuple_element_t<sizeof...(Ts) - 1, std::tuple<Ts...>>;

/**
 * @brief Get the type of the last value of variable template parameter pack.
 * @details
 * For example, `first_t_of<a, b, c, d>` is `decltype(d)`.
 * @tparam Vs the variable template parameter pack
 */
template <auto... Vs>
using last_t_of = std::remove_cvref_t<
    std::tuple_element_t<sizeof...(Vs) - 1, decltype(std::tuple{Vs...})>>;

/**
 * @brief Get the last value of variable template parameter pack.
 * @details
 * For example, `first_v<a, b, c, d>` is `d`.
 * @tparam Vs the variable template parameter pack
 */
template <auto... Vs>
constexpr auto last_v = (Vs, ...);

/**
 * @brief This concept is satisfied if all types of `Ts...` the same type.
 * @tparam Ts the class template parameter pack
 */
template <typename... Ts>
concept all_same = (std::same_as<first_t<Ts...>, Ts> && ...);
/**
 * @brief This concept is satisfied if all values of `Vs...` has the same type.
 * @tparam Vs the variable template parameter pack
 */
template <auto... Vs>
concept all_same_of = all_same<std::remove_cvref_t<decltype(Vs)>...>;

/**
 * @brief This concept is satisfied if `std::hash<T>` has specialization.
 * @tparam T any type
 */
template <typename T>
concept hashable = requires(std::hash<std::remove_cvref_t<T>> hash,
                            const std::remove_cvref_t<T> &obj) { hash(obj); };

/**
 * @brief This concept is satisfied if `T` is a view type to a constant
 * contiguous sequence of `Elem`.
 * @tparam T view type
 * @tparam Elem element type
 * @tparam SizeT size type of the sequence
 */
template <typename T, typename Elem, typename SizeT = std::size_t>
concept viewer =
    std::constructible_from<T, const Elem *, SizeT> && requires(const T &t) {
        t.begin();
        t.end();
        t.rbegin();
        t.rend();
        t.size();
        t[SizeT{}];
    };
}  // namespace meta

/**
 * @brief Some constexpr character operating functions.
 */
namespace charutils {
/**
 * @brief This concept is satisfied if `T` is a char-like type.
 * @details
 * It requires that the type must be a trivial type, can be compared in three-way,
 * and can be converted from `char` value.
 * @tparam T any char-like type
 */
template <typename T>
concept char_like = std::is_trivial_v<T> && std::equality_comparable<T> &&
                    std::three_way_comparable<T> &&
                    requires { T('\0') + T('\x20') - T('\x20'); };

/**
 * @brief `constexpr` re-implementation of `std::islower`.
 */
template <char_like T>
constexpr bool islower(T ch) {
    return ch >= T('a') && ch <= T('z');
}

/**
 * @brief `constexpr` re-implementation of `std::issuper`.
 */
template <char_like T>
constexpr bool issuper(T ch) {
    return ch >= T('A') && ch <= T('Z');
}

/**
 * @brief `constexpr` re-implementation of `std::isdigit`.
 */
template <char_like T>
constexpr bool isdigit(T ch) {
    return ch >= T('0') && ch <= T('9');
}

/**
 * @brief `constexpr` re-implementation of `std::isalpha`.
 */
template <char_like T>
constexpr bool isalpha(T ch) {
    return islower(ch) || issuper(ch);
}

/**
 * @brief `constexpr` re-implementation of `std::isalnum`.
 */
template <char_like T>
constexpr bool isalnum(T ch) {
    return isalpha(ch) || isdigit(ch);
}

/**
 * @brief `constexpr` re-implementation of `std::isxdigit`.
 */
template <char_like T>
constexpr bool isxdigit(T ch) {
    return isdigit(ch) || (ch >= T('a') && ch <= T('f')) ||
           (ch >= T('A') && ch <= T('F'));
}

/**
 * @brief `constexpr` re-implementation of `std::iscntrl`.
 */
template <char_like T>
constexpr bool iscntrl(T ch) {
    return (ch >= T('\0') && ch <= T('\x1F')) || ch == T('\x7F');
}

/**
 * @brief `constexpr` re-implementation of `std::ispunct`.
 */
template <char_like T>
constexpr bool ispunct(T ch) {
    return (ch >= T('!') && ch <= T('/')) || (ch >= T(':') && ch <= T('@')) ||
           (ch >= T('[') && ch <= T('`')) || (ch >= T('{') && ch <= T('~'));
}

/**
 * @brief `constexpr` re-implementation of `std::isblank`.
 */
template <char_like T>
constexpr bool isblank(T ch) {
    return ch == T(' ') || ch == T('\t');
}

/**
 * @brief `constexpr` re-implementation of `std::isspace`.
 */
template <char_like T>
constexpr bool isspace(T ch) {
    return isblank(ch) || ch == T('\f') || ch == T('\n') || ch == T('\r') ||
           ch == T('\v');
}

/**
 * @brief `constexpr` re-implementation of `std::isgraph`.
 */
template <char_like T>
constexpr bool isgraph(T ch) {
    return isalnum(ch) || ispunct(ch);
}

/**
 * @brief `constexpr` re-implementation of `std::isprint`.
 */
template <char_like T>
constexpr bool isprint(T ch) {
    return isgraph(ch) || ch == T(' ');
}

/**
 * @brief `constexpr` re-implementation of `std::toupper`.
 */
template <char_like T>
constexpr char toupper(T ch) {
    return ch >= T('a') && ch <= T('z') ? ch - T('\x20') : ch;
}

/**
 * @brief `constexpr` re-implementation of `std::tolower`.
 */
template <char_like T>
constexpr char tolower(T ch) {
    return ch >= T('A') && ch <= T('Z') ? ch + T('\x20') : ch;
}

/**
 * @brief Invert case of letter.
 */
template <char_like T>
constexpr char invert(T ch) {
    return ch >= T('a') && ch <= T('z')   ? ch - T('\x20')
           : ch >= T('A') && ch <= T('Z') ? ch + T('\x20')
                                          : ch;
}

/**
 * @brief Check if input character the same as one of variable template parameters.
 * @details
 * This function can be used as a unary predicate parameter for many standard library
 * algorithms in constant context. For example:
 * @code{.cpp}
 * constexpr auto iter = std::find_if(start, end, is<'a', 'b', 'c'>);
 * @endcode
 * @tparam Chs list of expected characters
 * @param ch characters to be checked
 * @return `true` if the input character the same as of variable template parameters.
 * @return `false` otherwise.
 */
template <char_like auto... Chs>
constexpr bool is(meta::first_t_of<Chs...> ch)
    requires meta::all_same_of<Chs...> && (sizeof...(Chs) > 1)
{
    return ((Chs == ch) || ...);
}

/**
 * @brief No matter what the input is, always output `Ch`.
 * @details
 * This function can be used as a unary operation parameter for many standard library
 * algorithms in constant context. For example:
 * @code{.cpp}
 * constexpr auto iter = std::transform(start, end, start, just<'0'>);
 * @endcode
 * @tparam Ch character to output
 * @param ignored any character
 * @return Value of `Ch`. 
 */
template <char_like auto Ch>
constexpr decltype(Ch) just([[maybe_unused]] decltype(Ch) ignored) {
    return Ch;
}

/**
 * @brief Cast input character to the value of another character type.
 * @tparam To output character type
 * @param from input character
 * @return Casted character.
 */
template <char_like To>
constexpr auto cast =
    [](char_like auto from) -> To { return static_cast<To>(from); };

/**
 * @brief
 * Replace the input character if it is the same as one of variable template parameters.
 * @details
 * For example, ``replace<'a', 'b', 'c', 'z'>(ch)`` will return ``'z'`` if `ch`
 * is ``'a'``, ``'b'`` or ``'c'``, otherwise return `ch` itself.
 * This function can be used as a unary operation parameter for many standard library
 * algorithms in constant context. For example:
 * @code{.cpp}
 * constexpr auto iter = std::transform(start, end, start, replace<'A', 'a'>);
 * @endcode
 * @tparam Chs list of expected characters
 * @param ch characters to be checked
 * @return The last value of variable template parameters if `ch` is one of them,
 * otherwise the value of `ch`.
 */
template <char_like auto... Chs>
constexpr meta::last_t_of<Chs...> replace(meta::last_t_of<Chs...> ch)
    requires meta::all_same_of<Chs...> && (sizeof...(Chs) > 1)
{
    return is<Chs...>(ch) ? meta::last_v<Chs...> : ch;
}

/**
 * @brief Replace the input character if it is NOT one of variable template parameters.
 * @details
 * It is the opposite function of `replace<Chs...>(ch)`.
 * For example, ``remain<'a', 'b', 'c', 'z'>(ch)`` will return ``'z'`` if `ch` is NOT
 * one of ``'a'``, ``'b'`` or ``'c'``, otherwise return `ch` itself.
 * This function can be used as a unary operation parameter for many standard library
 * algorithms in constant context. For example:
 * @code{.cpp}
 * constexpr auto iter = std::transform(start, end, start, remain<'-', '*'>);
 * @endcode
 * @tparam Chs list of expected characters
 * @param ch characters to be checked
 * @return The last value of variable template parameters if `ch` is NOT one of them,
 * otherwise the value of `ch`.
 */
template <char_like auto... Chs>
constexpr meta::last_t_of<Chs...> remain(meta::last_t_of<Chs...> ch)
    requires meta::all_same_of<Chs...> && (sizeof...(Chs) > 1)
{
    return is<Chs...>(ch) ? ch : meta::last_v<Chs...>;
}
}  // namespace charutils

template <std::size_t N, charutils::char_like T, typename view>
    requires meta::viewer<view, T>
struct cstr;

/**
 * @brief This concept is satisfied if `cstr` can be constructed from `T`.
 * @tparam T type that can be used to construct `cstr`
 */
template <typename T>
concept can_construct_cstr_from = requires(const T &t) { cstr{t}; };

/**
 * @brief This concept is satisfied if `cstr` can be constructed from `T` and the
 * `value_type` of the `cstr` is `CharT`.
 * @tparam T type that can be used to construct `cstr`
 * @tparam CharT expected `value_type` of `cstr`
 */
template <typename T, typename CharT>
concept can_construct_cstr_t_from =
    std::same_as<typename decltype(cstr(std::declval<T>()))::value_type, CharT>;

/**
 * @brief This concept is satisfied if `cstr` can be constructed from `T` and the
 * `value_type` of the `cstr` is `CharT` and length is `Len`.
 * @tparam T type that can be used to construct `cstr`
 * @tparam Len expected length of `cstr`
 * @tparam CharT expected `value_type` of `cstr`
 */
template <typename T, typename CharT, std::size_t Len>
concept can_construct_cstr_t_n_from =
    std::same_as<typename decltype(cstr(std::declval<T>()))::value_type,
                 CharT> &&
    (decltype(cstr(std::declval<T>()))::size() == Len);

/**
 * @brief String view type selector. Just an empty struct.
 * @tparam T string view type
 * @see sv
 */
template <typename T>
struct sv_selector {};

/**
 * @brief String view type selector. Assist in type deduction of `cstr`.
 * @details
 * For example:
 * 
 * @code{.cpp}
 * #include <span>
 * #include "conststr.hpp"
 * 
 * using conststr::cstr;
 * using conststr::sv;
 * 
 * // View type defaults to `std::string_view`, aka `std::basic_string_view<char>`
 * constexpr auto s1 = cstr("hello world!");
 * 
 * // View type is `std::span<const char>`
 * constexpr auto s2 = cstr("hello world!", sv<std::span<const char>>);
 * 
 * // You can also change the view type like this
 * constexpr auto s3 = cstr("hello world!").with_view<std::span<const char>>();
 * @endcode
 * @tparam T string view type
 */
template <typename T>
constexpr auto sv = sv_selector<T>{};

/**
 * @brief String type that can be evaluated in a constant context.
 * @details
 * Very similar to `std::array<char, N>`, but null terminator will be added.
 * @note When you iterate over it, the null terminator is not in range.
 * @tparam N size of string, without counting the null terminator
 * @tparam T character type, default to `char`
 * @tparam view view type of `T`, default to `std::basic_string_view<T>`
 */
template <std::size_t N, charutils::char_like T = char,
          typename view = std::basic_string_view<T>>
    requires meta::viewer<view, T>
struct cstr {
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using rreference = value_type &&;
    using const_rreference = const value_type &&;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = value_type *;
    using const_iterator = const value_type *;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using view_type = view;

    template <typename UnaryOperation>
    using operation_result =
        std::remove_cvref_t<std::invoke_result_t<UnaryOperation, value_type>>;

    /**
     * @brief Null terminator with the value ``'\0'``.
     */
    static constexpr value_type nul = value_type('\0');

    /**
     * @brief Indicator of the end of string.
     */
    static constexpr size_type npos = N;

    /**
     * @brief Default constructor, constructs string which is filled with ``'\0'``.
     * @param svs string view type selector, default view type is `std::basic_string_view`
     * @see sv
     */
    constexpr cstr(
        [[maybe_unused]] const sv_selector<view_type> &svs = {}) noexcept
        : _str{nul} {}

    /**
     * @brief Construct string from string literal.
     * @details
     * This is the most commonly used construction method. Just simply use `cstr("blabla")`
     * to construct the `cstr` without worrying about the template type.
     * @note
     * A simpler way is to use the user-defined string literal like `"blabla"_cs`.
     * @param str the input string array/literal
     * @param svs string view type selector, default view type is `std::basic_string_view`
     * @see conststr::literal
     * @see sv
     */
    constexpr cstr(
        const value_type (&str)[N + 1],
        [[maybe_unused]] const sv_selector<view_type> &svs = {}) noexcept {
        for (size_type i = 0; i < N; ++i) _str[i] = str[i];
        _str[N] = nul;
    }

    /**
     * @brief Construct string with `N` copies of character `ch`.
     * @param ch the character to fill
     * @param svs string view type selector, default view type is `std::basic_string_view`
     * @see sv
     */
    constexpr cstr(
        const value_type &ch,
        [[maybe_unused]] const sv_selector<view_type> &svs = {}) noexcept {
        fill(ch);
    }

    /**
     * @brief Constructor for variable template parameter pack expansion.
     * @details
     * For example, `cstr<sizeof...(chs)> s = { chs... };`.
     * @note A null terminator will be added to the end.
     */
    constexpr cstr(std::convertible_to<value_type> auto... vs)
        : _str{vs..., nul} {}

    /**
     * @brief Copy constructor.
     */
    constexpr cstr(const cstr &) = default;

    /**
     * @brief Copy from another string with different view type.
     */
    template <typename V2>
    constexpr cstr(const cstr<N, value_type, V2> &other) {
        std::copy_n(other.begin(), size(), begin());
        _str[N] = nul;
    };

    /**
     * @brief Destructor. In fact, it does nothing.
     */
    constexpr ~cstr() = default;

    /**
     * @brief Replaces the contents with a copy of another string.
     */
    template <can_construct_cstr_t_n_from<value_type, N> Str>
    constexpr cstr &operator=(const Str &str) {
        auto other = cstr(str);
        std::copy_n(other.begin(), size(), begin());
        return *this;
    };

    /**
     * @brief Fill the string with the character `ch`.
     * @param ch character to be used to fill the string
     */
    constexpr void fill(const value_type &ch) noexcept {
        std::fill_n(begin(), size(), ch);
        _str[N] = nul;
    }

    /**
     * @brief Swap the contents of this string with another string.
     * @tparam V2 view type of another string
     * @param other another string to swap the contents
     */
    template <typename V2>
    constexpr void swap(cstr<N, value_type, V2> &other) noexcept {
        std::swap_ranges(begin(), end(), other.begin());
    }

    /**
     * @brief Get size of string, without counting the null terminator.
     * @return The number of character element in the string.
     */
    static constexpr size_type size() noexcept { return N; }

    /**
     * @brief Alternative API of `size()`.
     * Get size of string, without counting the null terminator.
     * @return The number of character element in the string.
     */
    static constexpr size_type length() noexcept { return size(); }

    /**
     * @brief
     * Get the maximum number of characters the string is able to hold.
     * Always the same as `size()`.
     * @return Maximum number of characters.
     */
    static constexpr size_type max_size() noexcept { return size(); }

    /**
     * @brief Checks if string is empty.
     * @note
     * This means that the length ot string is 0 instead of the first character
     * is the null terminator.
     * @return `true` if string is empty.
     * @return `false` otherwise.
     */
    static constexpr bool empty() noexcept { return size() == 0; }

    /**
     * @brief
     * Get the pointer of the underlying array serving as the character storage.
     * @return A pointer to the underlying character storage.
     */
    constexpr pointer data() noexcept { return _str; }

    /**
     * @brief
     * Get the pointer of the underlying array serving as the character storage.
     * @return A pointer to the underlying character storage.
     */
    constexpr const_pointer data() const noexcept { return _str; }

    /**
     * @brief Get the pointer to a null-terminated character array of contents.
     * Always the same as `data()`.
     * @return A pointer to the underlying character storage.
     */
    constexpr const_pointer c_str() const noexcept { return data(); }

    /**
     * @brief Get an iterator to the first character of the string.
     * @return Iterator to the first character.
     */
    constexpr iterator begin() noexcept { return data(); }

    /**
     * @brief Get an iterator to the first character of the string.
     * @return Iterator to the first character.
     */
    constexpr const_iterator begin() const noexcept { return data(); }

    /**
     * @brief
     * Get an iterator to the character following the last character of the string.
     * @return Iterator to the character following the last character.
     */
    constexpr iterator end() noexcept { return data() + size(); }

    /**
     * @brief
     * Get an iterator to the character following the last character of the string.
     * @return Iterator to the character following the last character.
     */
    constexpr const_iterator end() const noexcept { return data() + size(); }

    /**
     * @brief Get an const iterator to the first character of the string.
     * @return Const iterator to the first character.
     */
    constexpr const_iterator cbegin() const noexcept { return data(); }

    /**
     * @brief
     * Get an const iterator to the character following the last character of the string.
     * @return Const iterator to the character following the last character.
     */
    constexpr const_iterator cend() const noexcept { return data() + size(); }

    /**
     * @brief Get a reverse iterator to the first character of the reversed string.
     * @return Reverse iterator to the first character.
     */
    constexpr reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    /**
     * @brief Get a reverse iterator to the first character of the reversed string.
     * @return Reverse iterator to the first character.
     */
    constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Get a reverse iterator to the character following the last character of
     * the reversed string.
     * @return Reverse iterator to the character following the last character.
     */
    constexpr reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    /**
     * @brief Get a reverse iterator to the character following the last character of
     * the reversed string.
     * @return Reverse iterator to the character following the last character.
     */
    constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief
     * Get a const reverse iterator to the first character of the reversed string.
     * @return Const reverse iterator to the first character.
     */
    constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Get a const reverse iterator to the character following the last character
     * of the reversed string.
     * @return Const reverse iterator to the character following the last character.
     */
    constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Get the reference to the character at specified location `pos`.
     * @param pos position of the character to return
     * @return Reference to a character.
     */
    constexpr reference operator[](size_type pos) noexcept { return _str[pos]; }

    /**
     * @brief Get the reference to the character at specified location `pos`.
     * @param pos position of the character
     * @return Reference to a character.
     */
    constexpr const_reference operator[](size_type pos) const noexcept {
        return _str[pos];
    }

    /**
     * @brief Tuple-like interface, extracts the Idx-th element from the string.
     * @tparam Idx position of the character
     * @return Reference to a character.
     */
    template <size_type Idx>
    constexpr reference get() & noexcept
        requires(N > Idx)
    {
        return _str[Idx];
    }

    /**
     * @brief Tuple-like interface, extracts the Idx-th element from the string.
     * @tparam Idx position of the character
     * @return Reference to a character.
     */
    template <size_type Idx>
    constexpr const_reference get() const & noexcept
        requires(N > Idx)
    {
        return _str[Idx];
    }

    /**
     * @brief Tuple-like interface, extracts the Idx-th element from the string.
     * @tparam Idx position of the character
     * @return R-value reference to a character.
     */
    template <size_type Idx>
    constexpr rreference get() && noexcept
        requires(N > Idx)
    {
        return std::move(_str[Idx]);
    }

    /**
     * @brief Tuple-like interface, extracts the Idx-th element from the string.
     * @tparam Idx position of the character
     * @return R-value reference to a character.
     */
    template <size_type Idx>
    constexpr const_rreference get() const && noexcept
        requires(N > Idx)
    {
        return std::move(_str[Idx]);
    }

    /**
     * @brief Get the reference to the first character in the string.
     * @note Reference to null terminator will be returned if string is empty.
     * @return Reference to the first character.
     */
    constexpr reference front() noexcept { return *begin(); }

    /**
     * @brief Get the reference to the first character in the string.
     * @note Reference to null terminator will be returned if string is empty.
     * @return Reference to the first character.
     */
    constexpr const_reference front() const noexcept { return *cbegin(); }

    /**
     * @brief Get the reference to the last character in the string.
     * @note Reference to null terminator will be returned if string is empty.
     * @return Reference to the last character.
     */
    constexpr reference back() noexcept {
        if constexpr (N > 0)
            return *(end() - 1);
        else
            return *end();
    }

    /**
     * @brief Get the reference to the last character in the string.
     * @note Reference to null terminator will be returned if string is empty.
     * @return Reference to the last character.
     */
    constexpr const_reference back() const noexcept {
        if constexpr (N > 0)
            return *(cend() - 1);
        else
            return *cend();
    }

    /**
     * @brief Concatenate the contents with another string.
     * @tparam Str type of another string
     * @param str another string to be concatenated
     * @return New string containing the concatenated contents of two strings.
     */
    template <can_construct_cstr_t_from<value_type> Str>
    constexpr auto operator+(const Str &str) const noexcept {
        return append(str);
    }

    /**
     * @brief Check if the contents of two strings are the same.
     * @tparam N2 size of another string
     * @tparam V2 view type of another string
     * @param other another string to be compared
     * @return `false` always in this overloaded version.
     */
    template <std::size_t N2, typename V2>
    constexpr bool operator==(
        const cstr<N2, value_type, V2> &other) const noexcept {
        return false;
    }

    /**
     * @brief Check if the contents of two strings are the same.
     * @tparam V2 view type of another string
     * @param other another string to be compared
     * @return `true` if the contents of two strings are the same.
     * @return `false` otherwise.
     */
    template <typename V2>
    constexpr bool operator==(
        const cstr<N, value_type, V2> &other) const noexcept {
        if constexpr (std::equality_comparable<view_type>)
            return this->operator view_type() == other.operator view_type();
        else
            return std::equal(cbegin(), cend(), other.cbegin(), other.cend());
    }

    /**
     * @brief Check if the contents of two strings are the same.
     * @param other another string to be compared
     * @return `true` if the contents of two strings are the same.
     * @return `false` otherwise.
     */
    constexpr bool operator==(const view_type &other) const noexcept {
        if constexpr (std::equality_comparable<view_type>)
            return this->operator view_type() == other;
        else
            return std::equal(cbegin(), cend(), other.begin(), other.end());
    }

    /**
     * @brief Compares the contents of two strings lexicographically.
     * @note Do not use `s1 <=> s2` directly, this operator will automatically derive
     * other logical operators: `>`, `<`, `>=` and `<=`.
     * @tparam N2 size of another string
     * @tparam V2 view type of another string
     * @param other another string to be compared
     * @return Ordering of two strings.
     */
    template <std::size_t N2, typename V2>
    constexpr auto operator<=>(const cstr<N2, value_type, V2> &other) noexcept {
        if constexpr (std::three_way_comparable<view_type>)
            return this->operator view_type() <=> other.operator view_type();
        else
            return std::lexicographical_compare_three_way(
                cbegin(), cend(), other.cbegin(), other.cend());
    }

    /**
     * @brief Compares the contents of two strings lexicographically.
     * @note Do not use `s1 <=> s2` directly, this operator will automatically derive
     * other logical operators: `>`, `<`, `>=` and `<=`.
     * @param other another string to be compared
     * @return Ordering of two strings.
     */
    constexpr auto operator<=>(const view_type &other) const noexcept {
        if constexpr (std::three_way_comparable<view_type>)
            return this->operator view_type() <=> other;
        else
            return std::lexicographical_compare_three_way(
                cbegin(), cend(), other.begin(), other.end());
    }

    /**
     * @brief Implicit conversion to the view type.
     */
    constexpr operator view_type() const noexcept {
        return view_type(data(), size());
    }

    /**
     * @brief Change view type.
     * @tparam V2 new view type
     * @return String with new view type
     */
    template <meta::viewer<value_type, size_type> V2>
    constexpr auto with_view() const noexcept -> cstr<N, value_type, V2> {
        return cstr<N, value_type, V2>{*this};
    }

    /**
     * @brief Get the substring.
     * @tparam Start position of the first character 
     * @tparam Len length of the substring
     * @return The substring [`Start`, `Start + Len`) or [`Start`, `size()`).
     */
    template <size_type Start, size_type Len = N>
        requires(N >= Start)
    constexpr auto substr() const noexcept {
        constexpr size_type size = std::min(Start + Len, N) - Start;
        cstr<size, value_type, view_type> ret{};
        for (size_type i = 0; i < size; ++i) ret[i] = _str[Start + i];
        return ret;
    };

    /**
     * @brief Get the substring containing first `Len` characters.
     * Always be equivalent to `substr<0, Len>()`.
     * @tparam Len length of the sub string.
     * @return The substring [0, `Len`) or [0, `size()`).
     */
    template <size_type Len>
    constexpr auto cut() const noexcept -> cstr<Len, value_type, view_type> {
        return substr<0, Len>();
    }

    /**
     * @brief Remove `Len` bytes of prefix. Always be equivalent to `substr<Len>()`.
     * @tparam Len bytes of prefix to remove
     * @return The substring [`Len`, `size()`).
     */
    template <size_type Len>
    constexpr auto remove_prefix() const noexcept {
        return substr<Len>();
    }

    /**
     * @brief Remove `Len` bytes of suffix. Always be equivalent to
     * `substr<0, size() - Len>()`.
     * @tparam Len bytes of suffix to remove
     * @return The substring [0, `size() - Len`).
     */
    template <size_type Len>
    constexpr auto remove_suffix() const noexcept {
        return substr<0, N - Len>();
    }

    /**
     * @brief Insert `Count` copies of character `ch` at the position `pos`.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @tparam Count count of insertion, default to 1
     * @param pos position that the characters will be inserted
     * @param ch character to insert
     * @return The modified string.
     */
    template <size_type Count = 1>
    constexpr auto insert(size_type pos, const value_type &ch) const noexcept
        -> cstr<N + Count, value_type, view_type> {
        if (pos > N) pos = N;
        if constexpr (Count == 0)
            return *this;
        else {
            cstr<N + Count, value_type, view_type> ret{};
            for (size_type i = 0; i < pos; ++i) ret[i] = _str[i];
            for (size_type i = pos; i < pos + Count; ++i) ret[i] = ch;
            for (size_type i = pos; i < N; ++i) ret[i + Count] = _str[i];
            return ret;
        }
    };

    /**
     * @brief Insert `Count` copies of character `ch` before iterator `iter`.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @tparam Count count of insertion, default to 1
     * @param iter iterator before which the characters will be inserted
     * @param ch character to insert
     * @return The modified string.
     */
    template <size_type Count = 1>
    constexpr auto insert(const_iterator iter,
                          const value_type &ch) const noexcept
        -> cstr<N + Count, value_type, view_type> {
        size_type pos = to_pos(iter);
        if constexpr (Count == 0)
            return *this;
        else {
            cstr<N + Count, value_type, view_type> ret{};
            for (size_type i = 0; i < pos; ++i) ret[i] = _str[i];
            for (size_type i = pos; i < pos + Count; ++i) ret[i] = ch;
            for (size_type i = pos; i < N; ++i) ret[i + Count] = _str[i];
            return ret;
        }
    };

    /**
     * @brief Insert another string at the position `pos`.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @tparam Str type of another string
     * @param pos position that the characters will be inserted
     * @param str string to insert
     * @return The modified string.
     */
    template <can_construct_cstr_t_from<value_type> Str>
    constexpr auto insert(size_type pos, const Str &str) const noexcept {
        if (pos > N) pos = N;

        auto other = conststr::cstr(str);
        constexpr size_type N2 = decltype(other)::size();
        cstr<N + N2, value_type, view_type> ret{};
        for (size_type i = 0; i < pos; ++i) ret[i] = _str[i];
        for (size_type i = 0; i < N2; ++i) ret[i + pos] = other[i];
        for (size_type i = pos; i < N; ++i) ret[i + N2] = _str[i];
        return ret;
    }

    /**
     * @brief Insert another string before iterator `iter`.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @tparam Str type of another string
     * @param iter iterator before which the characters will be inserted
     * @param str string to insert
     * @return The modified string.
     */
    template <can_construct_cstr_t_from<value_type> Str>
    constexpr auto insert(const_iterator iter, const Str &str) const noexcept {
        size_type pos = to_pos(iter);
        auto other = conststr::cstr(str);
        constexpr size_type N2 = decltype(other)::size();
        cstr<N + N2, value_type, view_type> ret{};
        for (size_type i = 0; i < pos; ++i) ret[i] = _str[i];
        for (size_type i = 0; i < N2; ++i) ret[i + pos] = other[i];
        for (size_type i = pos; i < N; ++i) ret[i + N2] = _str[i];
        return ret;
    }

    /**
     * @brief Append `Count` copies of character `ch` at the end.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @tparam Count count of appending, default to 1
     * @param ch character to append
     * @return The modified string.
     */
    template <size_type Count = 1>
    constexpr auto append(const value_type &ch) const noexcept
        -> cstr<N + Count, value_type, view_type> {
        if constexpr (Count == 0)
            return *this;
        else {
            cstr<N + Count, value_type, view_type> ret{};
            for (size_type i = 0; i < N; ++i) ret[i] = _str[i];
            for (size_type i = N; i < N + Count; ++i) ret[i] = ch;
            return ret;
        }
    }

    /**
     * @brief Append another string at the end.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @tparam Str type of another string
     * @param str another string to append
     * @return The modified string.
     */
    template <can_construct_cstr_t_from<value_type> Str>
    constexpr auto append(const Str &str) const noexcept {
        return flatten(str);
    }

    /**
     * @brief Flatten multiple strings of different lengths.
     * @note
     * Use the out-of-class `flatten()` function to better suit metaprogramming.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @param strs strings to be flattened
     * @return Concatenated string.
     */
    template <size_type... Sizes>
    constexpr auto flatten(
        const cstr<Sizes, value_type, view_type> &...strs) const noexcept {
        if constexpr (sizeof...(Sizes) == 0)
            return *this;
        else {
            constexpr size_type COUNT = sizeof...(strs) + 1;
            constexpr size_type LENGTHS[COUNT] = {N, Sizes...};
            constexpr size_type FLAT_LENGTH = N + (Sizes + ...);

            cstr<FLAT_LENGTH, value_type, view_type> flat{};
            const_pointer datas[COUNT] = {data(), strs.data()...};
            size_type index = 0;
            for (size_type i = 0; i < COUNT; i++)
                for (size_type j = 0; j < LENGTHS[i]; j++)
                    flat[index++] = datas[i][j];

            return flat;
        }
    }

    /**
     * @brief Flatten multiple strings of different lengths.
     * @note
     * Use the out-of-class `flatten()` function to better suit metaprogramming.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @tparam Strs types of strings
     * @param strs strings to be flattened
     * @return Concatenated string.
     */
    template <can_construct_cstr_t_from<value_type>... Strs>
    constexpr auto flatten(const Strs &...strs) const noexcept {
        return flatten(conststr::cstr(strs)...);
    }

    /**
     * @brief Replaces the characters in the range [`pos`, `min(pos + count, size())`)
     * with character `ch`.
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string. If you really want to modify current string, use
     * `std::transform(first, last, first, charutils::just<'a'>)` while `first` and
     * `last` are iterators of current string.
     * @param pos start of the substring that is going to be replaced
     * @param ch character value to use for replacement
     * @param count length of the substring that is going to be replaced
     * @return The modified string.
     */
    constexpr auto replace(size_type pos, const value_type &ch,
                           size_type count = 1) const noexcept
        -> cstr<N, value_type, view_type> {
        if (pos > N) pos = N;
        cstr<N, value_type, view_type> ret = *this;
        size_type end = std::min(pos + count, N);
        for (size_type i = pos; i < end; ++i) ret[i] = ch;
        return ret;
    }

    /**
     * @brief Replaces the characters in the range [`max(first, cbegin())`,
     * `min(last, cend())`)  with character `ch`.
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string. If you really want to modify current string, use
     * `std::transform(first, last, first, charutils::just<'a'>)` while `first` and
     * `last` are iterators of current string.
     * @param first begin of range of characters that is going to be replaced
     * @param last end of range of characters that is going to be replaced
     * @param ch character value to use for replacement
     * @return The modified string.
     */
    constexpr auto replace(const_iterator first, const_iterator last,
                           const value_type &ch) const noexcept
        -> cstr<N, value_type, view_type> {
        auto [pos, count] = to_pos_len(first, last);
        cstr<N, value_type, view_type> ret = *this;
        size_type end = std::min(pos + count, N);
        for (size_type i = pos; i < end; ++i) ret[i] = ch;
        return ret;
    }

    /**
     * @brief Replaces the characters in the range [`pos`,
     * `min(pos + str.size() * count, size())`) with another string `str`.
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string.
     * @param pos start of the substring that is going to be replaced
     * @param str string to use for replacement
     * @param count replacement times of `str`
     * @return The modified string.
     */
    constexpr auto replace(size_type pos, const view_type &str,
                           size_type count = 1) const noexcept
        -> cstr<N, value_type, view_type> {
        if (pos > N) pos = N;
        cstr<N, value_type, view_type> ret = *this;
        size_type end = std::min(pos + count * str.size(), N);
        for (size_type i = 0; i < end - pos; ++i) ret[i] = str[i % str.size()];
        return ret;
    }

    /**
     * @brief Replaces the characters in the range [`max(first, cbegin())`,
     * `min(last, cend())`) with another string `str`.
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string.
     * @param first begin of range of characters that is going to be replaced
     * @param last end of range of characters that is going to be replaced
     * @param str string to use for replacement
     * @return The modified string.
     */
    constexpr auto replace(const_iterator first, const_iterator last,
                           const view_type &str) const noexcept
        -> cstr<N, value_type, view_type> {
        auto [pos, count] = to_pos_len(first, last);
        return replace(pos, str, count);
    }

    /**
     * @brief Replaces the characters in the range [`pos`, `min(pos + len, size())`)
     * according to variable template parameters. 
     * @details
     * For example, ```str.replace<'a', 'b', 'c', 'd'>(...)``` replaces ```'a'```, ```'b'```
     * and ```'c'``` in the contents of string to ```'d'```. Always be equivalent to
     * `transform(charutils::replace<Chs...>, pos, len)`.
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string. If you really want to modify current string, use
     * `std::transform(first, last, first, charutils::replace<Chs...>)` while `first` and
     * `last` are iterators of current string.
     * @tparam Chs characters for replacement, see the example
     * @param pos start of the substring that is going to be replaced 
     * @param len length of the substring that is going to be replaced
     * @return The modified string.
     */
    template <value_type... Chs>
    constexpr auto replace(size_type pos = 0, size_type len = N) const noexcept
        -> cstr<N, value_type, view_type>
        requires(sizeof...(Chs) > 0)
    {
        return transform(charutils::replace<Chs...>, pos, len);
    }

    /**
     * @brief Replaces the characters in the range [`max(first, cbegin())`,
     * `min(last, cend())`) according to variable template parameters. 
     * @details
     * For example, ```str.replace<'a', 'b', 'c', 'd'>(...)``` replaces ```'a'```, ```'b'```
     * and ```'c'``` in the contents of string to ```'d'```. Always be equivalent to
     * `transform(charutils::replace<Chs...>, first, last)`.
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string. If you really want to modify current string, use
     * `std::transform(first, last, first, charutils::replace<Chs...>)` while `first` and
     * `last` are iterators of current string.
     * @tparam Chs characters for replacement, see the example
     * @param first begin of range of characters that is going to be replaced
     * @param last end of range of characters that is going to be replaced
     * @return The modified string.
     */
    template <value_type... Chs>
    constexpr auto replace(const_iterator first,
                           const_iterator last) const noexcept
        -> cstr<N, value_type, view_type>
        requires(sizeof...(Chs) > 0)
    {
        return transform(charutils::replace<Chs...>, first, last);
    }

    /**
     * @brief Remove the character at position `pos`.
     * @note
     * If `pos` is greater than or equal to `size()`, the last character will be removed.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @param pos position of the character to be removed
     * @return The modified string.
     */
    constexpr auto erase(size_type pos) const noexcept
        -> cstr<N - 1, value_type, view_type> {
        if (pos >= N) return pop();
        cstr<N - 1, value_type, view_type> ret{};
        for (size_type i = 0; i < pos; ++i) ret[i] = _str[i];
        for (size_type i = pos; i < N - 1; ++i) ret[i] = _str[i + 1];
        return ret;
    }

    /**
     * @brief Remove the character pointed by `iter`.
     * @note
     * If `iter` is out-of-range, the first or the last character will be removed.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @param iter iterator which points to the character that will be removed
     * @return The modified string.
     */
    constexpr auto erase(const_iterator iter) const noexcept
        -> cstr<N - 1, value_type, view_type> {
        size_type pos = to_pos(iter);
        cstr<N - 1, value_type, view_type> ret{};
        for (size_type i = 0; i < pos; ++i) ret[i] = _str[i];
        for (size_type i = pos; i < N - 1; ++i) ret[i] = _str[i + 1];
        return ret;
    }

    /**
     * @brief Push a character to the end of string. Always be equivalent to `append(ch)`.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @param ch character to be pushed
     * @return The modified string.
     */
    constexpr auto push(const value_type &ch) const noexcept
        -> cstr<N + 1, value_type> {
        return append(ch);
    }

    /**
     * @brief Pop the last character from the end of string.
     * Always be equivalent to `cut<size() - 1>()`.
     * @note This method changes the string length, therefore it does not change `*this`,
     * but returns the modified string.
     * @return The modified string.
     */
    constexpr auto pop() const noexcept -> cstr<N - 1, value_type, view_type> {
        return cut<N - 1>();
    }

    /**
     * @brief Apply the given function to each character in the range [`pos`,
     * `min(pos + len, size())`).
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string. If you really want to modify current string,
     * use `std::transform(str.begin(), str.end(), str.begin(), op)`.
     * @tparam UnaryOperation type of the unary operation function
     * @tparam V view type for the transform result, by default, if return type `Ret`
     * of the unary operation `op` is the same as the original value type,
     * then the view type is left unchanged, otherwise it is `std::basic_string_view<Ret>`
     * @param op unary operation function object that will be applied, the signature
     * of the function should be `Ret f(const T &)` or `Ret f(T)`.
     * @param pos start position of the range
     * @param len count of character to be handled
     * @param svs string view type selector
     * @return The generated string.
     * @see sv
     */
    template <typename UnaryOperation, typename V2 = void>
    constexpr auto transform(
        UnaryOperation op, size_type pos = 0, size_type len = N,
        [[maybe_unused]] const sv_selector<V2> &svs = {}) const noexcept
        requires(meta::viewer<V2, operation_result<UnaryOperation>> ||
                 (std::is_void_v<V2>))
    {
        using result_t = operation_result<UnaryOperation>;
        using view_t = std::conditional_t<
            std::is_void_v<V2>,
            std::conditional_t<std::is_same_v<value_type, result_t>, view_type,
                               std::basic_string_view<result_t>>,
            V2>;
        if (pos > N) pos = N;
        cstr<N, result_t, view_t> ret{};
        auto begin = cbegin() + pos;
        auto end = cbegin() + std::min(pos + len, N);
        auto begin2 = ret.begin() + pos;
        std::transform(begin, end, begin2, op);
        return ret;
    }

    /**
     * @brief Apply the given function to each character in the range [`first`,
     * `min(last, cend())`).
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string. If you really want to modify current string,
     * use `std::transform(str.begin(), str.end(), str.begin(), op)`.
     * @tparam UnaryOperation type of the unary operation function
     * @tparam V2 view type for the transform result, by default, if return type `Ret`
     * of the unary operation `op` is the same as the original value type,
     * then the view type is left unchanged, otherwise it is `std::basic_string_view<Ret>`
     * @param op unary operation function object that will be applied, the signature
     * of the function should be `Ret f(const T &)` or `Ret f(T)`.
     * @param first iterator to the start position of the range
     * @param last iterator to the end position of the range
     * @param svs string view type selector
     * @return The generated string.
     * @see sv
     */
    template <typename UnaryOperation, typename V2 = void>
    constexpr auto transform(
        UnaryOperation op, const_iterator first, const_iterator last,
        [[maybe_unused]] const sv_selector<V2> &svs = {}) const noexcept
        requires(meta::viewer<V2, operation_result<UnaryOperation>> ||
                 (std::is_void_v<V2>))
    {
        using result_t = operation_result<UnaryOperation>;
        using view_t = std::conditional_t<
            std::is_void_v<V2>,
            std::conditional_t<std::is_same_v<value_type, result_t>, view_type,
                               std::basic_string_view<result_t>>,
            V2>;
        first = std::max(first, cbegin());
        last = std::min(last, cend());
        cstr<N, result_t, view_t> ret{};
        auto first2 = ret.begin() + (first - cbegin());
        std::transform(first, last, first2, op);
        return ret;
    }

    /**
     * @brief Convert character type. Always be equivalent to
     * `transform(charutils::cast<T2>, 0, N, sv<V2>)`.
     * @tparam T2 another character type
     * @tparam V2 view type of output string
     * @param svs string view type selector
     * @return String with another character type.
     * @see sv
     */
    template <charutils::char_like T2, typename V2 = void>
    constexpr auto cast(
        [[maybe_unused]] const sv_selector<V2> &svs = {}) const noexcept
        requires(meta::viewer<V2, T2> || (std::is_void_v<V2>))
    {
        return transform(charutils::cast<T2>, 0, N, sv<V2>);
    }

    /**
     * @brief Convert all characters to uppercase.
     * Always be equivalent to `transform(charutils::toupper<value_type>)`.
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string. If you really want to modify current string, use
     * `std::transform(str.begin(), str.end(), str.begin(), charutils::toupper<T>)`.
     * @return The modified string. 
     */
    constexpr auto uppercase() const noexcept
        -> cstr<N, value_type, view_type> {
        return transform(charutils::toupper<value_type>);
    }

    /**
     * @brief Convert all characters to lowercase.
     * Always be equivalent to `transform(charutils::tolower<value_type>)`.
     * @note To maintain API uniformity, this method does not change `*this`,
     * but returns the modified string. If you really want to modify current string, use
     * `std::transform(str.begin(), str.end(), str.begin(), charutils::tolower<T>)`.
     * @return The modified string. 
     */
    constexpr auto lowercase() const noexcept
        -> cstr<N, value_type, view_type> {
        return transform(charutils::tolower<value_type>);
    }

    /**
     * @brief Find the first character equal to the given character.
     * @param ch character to search for
     * @param pos position at which to start the search
     * @return Position of the found character or `npos` if not found.
     */
    constexpr size_type find(const value_type &ch,
                             size_type pos = 0) const noexcept {
        if (pos >= npos) return npos;
        return to_pos(std::find(cbegin() + pos, cend(), ch));
    }

    /**
     * @brief Find the first substring equal to the given substring.
     * @param str substring to search for
     * @param pos position at which to start the search
     * @return Position of the first character of the found substring or `npos` if not found.
     */
    constexpr size_type find(const view_type &str,
                             size_type pos = 0) const noexcept {
        if (pos >= npos) return npos;
        return to_pos(
            std::search(cbegin() + pos, cend(), str.cbegin(), str.cend()));
    }

    /**
     * @brief Find the last character equal to the given character.
     * @param ch character to search for
     * @param pos position at which to start the search
     * @return Position of the found character or `npos` if not found.
     */
    constexpr size_type rfind(const value_type &ch,
                              size_type pos = npos) const noexcept {
        auto beg =
            pos >= npos ? crbegin() : const_reverse_iterator(cbegin() + pos);
        auto iter = std::find(beg, crend(), ch);
        return iter == crend() ? npos : to_pos(iter.base() - 1);
    }

    /**
     * @brief Find the last substring equal to the given substring.
     * @param str substring to search for
     * @param pos position at which to start the search
     * @return Position of the first character of the found substring or `npos` if not found.
     */
    constexpr size_type rfind(const view_type &str,
                              size_type pos = npos) const noexcept {
        auto beg =
            pos >= npos ? crbegin() : const_reverse_iterator(cbegin() + pos);
        auto iter = std::search(beg, crend(), str.crbegin(), str.crend());
        return iter == crend() ? npos : to_pos(iter.base() - str.size());
    }

    /**
     * @brief Find the first character for which predicate `p` returns `true`.
     * @tparam UnaryPredicate type of the unary predicate function
     * @param p unary predicate function object which returns `true` for the required element.
     * @param pos position at which to start the search
     * @return Position of the found character or `npos` if not found.
     */
    template <typename UnaryPredicate>
    constexpr size_type find_if(UnaryPredicate p,
                                size_type pos = 0) const noexcept {
        if (pos >= npos) return npos;
        return to_pos(std::find_if(cbegin() + pos, cend(), p));
    }

    /**
     * @brief Find the last character for which predicate `p` returns `true`.
     * @tparam UnaryPredicate type of the unary predicate function
     * @param p unary predicate function object which returns `true` for the required element.
     * @param pos position at which to start the search
     * @return Position of the found character or `npos` if not found.
     */
    template <typename UnaryPredicate>
    constexpr size_type rfind_if(UnaryPredicate p,
                                 size_type pos = npos) const noexcept {
        auto beg =
            pos >= npos ? crbegin() : const_reverse_iterator(begin() + pos);
        auto iter = std::find_if(beg, crend(), p);
        return iter == crend() ? npos : to_pos(iter.base() - 1);
    }

    /**
     * @brief Check if the string begins with the given character.
     * @param ch any character
     * @return `true` if the string begins with the provided character.
     * @return `false` otherwise.
     */
    constexpr bool starts_with(const value_type &ch) const noexcept {
        if constexpr (N == 0)
            return false;
        else
            return front() == ch;
    }

    /**
     * @brief Check if the string begins with the given substring.
     * @param str any string
     * @return `true` if the string begins with the provided substring.
     * @return `false` otherwise.
     */
    constexpr bool starts_with(const view_type &str) const noexcept {
        if constexpr (requires { this->operator view_type().starts_with(str); })
            return this->operator view_type().starts_with(str);
        else if constexpr (std::equality_comparable<view_type>)
            return size() > str.size() && view_type(begin(), str.size()) == str;
        else
            return size() > str.size() &&
                   std::equal(str.begin(), str.end(), begin());
    }

    /**
     * @brief Check if the string ends with the given character.
     * @param ch any character
     * @return `true` if the string ends with the provided character.
     * @return `false` otherwise.
     */
    constexpr bool ends_with(const value_type &ch) const noexcept {
        if constexpr (N == 0)
            return false;
        else
            return back() == ch;
    }

    /**
     * @brief Check if the string ends with the given substring.
     * @param str any string
     * @return `true` if the string ends with the provided substring.
     * @return `false` otherwise.
     */
    constexpr bool ends_with(const view_type &str) const noexcept {
        if constexpr (requires { this->operator view_type().ends_with(str); })
            return this->operator view_type().ends_with(str);
        else if constexpr (std::equality_comparable<view_type>)
            return size() > str.size() &&
                   view_type(end() - str.size(), str.size()) == str;
        else
            return size() > str.size() &&
                   std::equal(str.begin(), str.end(), end() - str.size());
    }

    /**
     * @brief Check if the string contains the given character.
     * Always be equivalent to `find(ch) != npos`.
     * @param ch any character
     * @return `true` if the string contains the provided character.
     * @return `false` otherwise.
     */
    constexpr bool contains(const value_type &ch) const noexcept {
        return find(ch) != npos;
    }

    /**
     * @brief Check if the string contains the given substring.
     * Always be equivalent to `find(str) != npos`.
     * @param str any string
     * @return `true` if the string contains the provided substring.
     * @return `false` otherwise.
     */
    constexpr bool contains(const view_type &str) const noexcept {
        return find(str) != npos;
    }

    /**
     * @brief Underlying storage. Unless absolutely necessary,
     * you should not access it directly. Instead, you should use `begin()` and `end()`.
     */
    value_type _str[N + 1];

   private:
    inline constexpr size_t to_pos(const_iterator iter) const noexcept {
        return std::min(size_t(std::max(iter, cbegin()) - cbegin()), N);
    }

    inline constexpr std::tuple<size_t, size_t> to_pos_len(
        const_iterator first, const_iterator last) const noexcept {
        size_t first_pos = to_pos(first);
        size_t last_pos = to_pos(last);
        return {first_pos, std::max(first_pos, last_pos) - first_pos};
    }
};

/**
 * @brief Deduction guide for `cstr` with default view type.
 */
template <charutils::char_like T, std::size_t N>
cstr(const T (&)[N]) -> cstr<N - 1, T, std::basic_string_view<T>>;

/**
 * @brief Deduction guide for `cstr` with string view type selector.
 */
template <charutils::char_like T, std::size_t N, meta::viewer<T> V>
cstr(const T (&)[N], const sv_selector<V> &) -> cstr<N - 1, T, V>;

/**
 * @brief Deduction guide for `cstr` with default view type.
 */
template <charutils::char_like T>
cstr(const T &ch) -> cstr<1, T, std::basic_string_view<T>>;

/**
 * @brief Deduction guide for `cstr` with string view type selector.
 */
template <charutils::char_like T, meta::viewer<T> V>
cstr(const T &ch, const sv_selector<V> &) -> cstr<1, T, V>;

/**
 * @brief Tuple-like interface, extracts the Idx-th element from the string.
 * @tparam Idx position of the character
 * @param str string to be operated
 * @return Reference to a character.
 */
template <std::size_t Idx, charutils::char_like T, typename U, std::size_t N>
typename cstr<N, T, U>::reference get(cstr<N, T, U> &str) noexcept {
    return str.template get<Idx>();
}

/**
 * @brief Tuple-like interface, extracts the Idx-th element from the string.
 * @tparam Idx position of the character
 * @param str string to be operated
 * @return Reference to a character.
 */
template <std::size_t Idx, charutils::char_like T, typename U, std::size_t N>
typename cstr<N, T, U>::const_reference get(const cstr<N, T, U> &str) noexcept {
    return str.template get<Idx>();
}

/**
 * @brief Tuple-like interface, extracts the Idx-th element from the string.
 * @tparam Idx position of the character
 * @param str string to be operated
 * @return R-value reference to a character.
 */
template <std::size_t Idx, charutils::char_like T, typename U, std::size_t N>
typename cstr<N, T, U>::rreference get(cstr<N, T, U> &&str) noexcept {
    return str.template get<Idx>();
}

/**
 * @brief Tuple-like interface, extracts the Idx-th element from the string.
 * @tparam Idx position of the character
 * @param str string to be operated
 * @return R-value reference to a character.
 */
template <std::size_t Idx, charutils::char_like T, typename U, std::size_t N>
const typename cstr<N, T, U>::const_rreference get(
    const cstr<N, T, U> &&str) noexcept {
    return str.template get<Idx>();
}

/**
 * @brief Satisfy the named requirement `Swappable`.
 * @param lhs the value to be swapped
 * @param rhs the value to be swapped
 */
template <charutils::char_like T, typename V1, typename V2, std::size_t N>
constexpr void swap(cstr<N, T, V1> &lhs, cstr<N, T, V2> &rhs) {
    lhs.swap(rhs);
}

/**
 * @brief Flatten multiple strings of different lengths.
 * @details
 * Compared to `str1.flatten(str2, str3 ...)`, the form `flatten(str1, str2, str3 ...)`
 * is more suitable for metaprogramming.
 * @param first first string to flattened
 * @param strs other strings to flattened
 * @return constexpr auto 
 */
template <can_construct_cstr_from First, can_construct_cstr_from... Str>
constexpr auto flatten(const First &first, const Str &...strs)
    requires meta::all_same<
        typename decltype(cstr(std::declval<First>()))::value_type,
        typename decltype(cstr(std::declval<Str>()))::value_type...>
{
    return cstr(first).flatten(strs...);
}

/**
 * @brief Output string to std::ostream
 */
template <charutils::char_like T, typename U, std::size_t N>
std::ostream &operator<<(std::ostream &os, const cstr<N, T, U> &str) {
    os << static_cast<typename cstr<N, T, U>::view_type>(str);
    return os;
}

/**
 * @brief Define string literal suffix.
 */
namespace literal {
/**
 * @brief User-defined string literal with suffix `_cs`.
 * @details
 * Simply `using` this namespace to create `cstr` through `'a'_cs`.
 */
consteval auto operator""_cs(char ch) { return cstr<1, char>(ch); }

/**
 * @brief User-defined string literal with suffix `_cs`.
 * @details
 * Simply `using` this namespace to create `cstr` through `u8'a'_cs`.
 */
consteval auto operator""_cs(char8_t ch) { return cstr<1, char8_t>(ch); }

/**
 * @brief User-defined string literal with suffix `_cs`.
 * @details
 * Simply `using` this namespace to create `cstr` through `u'a'_cs`.
 */
consteval auto operator""_cs(char16_t ch) { return cstr<1, char16_t>(ch); }

/**
 * @brief User-defined string literal with suffix `_cs`.
 * @details
 * Simply `using` this namespace to create `cstr` through `U'a'_cs`.
 */
consteval auto operator""_cs(char32_t ch) { return cstr<1, char32_t>(ch); }

/**
 * @brief User-defined string literal with suffix `_cs`.
 * @details
 * Simply `using` this namespace to create `cstr` through `L'a'_cs`.
 */
consteval auto operator""_cs(wchar_t ch) { return cstr<1, wchar_t>(ch); }

/**
 * @brief User-defined string literal with suffix `_cs`.
 * @details
 * Simply `using` this namespace to create `cstr` through `"blabla"_cs`.
 */
template <cstr str>
consteval auto operator""_cs() {
    return str;
}
}  // namespace literal

}  // namespace conststr

/**
 * @brief Specialize `std::tuple_size` make `cstr` tuple-like.
 */
template <conststr::charutils::char_like T, typename U, std::size_t N>
struct std::tuple_size<conststr::cstr<N, T, U>>
    : public std::integral_constant<std::size_t, N> {};

/**
 * @brief Specialize `std::tuple_element` make `cstr` tuple-like.
 */
template <std::size_t Idx, conststr::charutils::char_like T, typename U,
          std::size_t N>
struct std::tuple_element<Idx, conststr::cstr<N, T, U>> {
    static_assert(N > Idx);
    using type = T;
};

/**
 * @brief Specialize `std::hash` for `cstr`, depend on the view type of `cstr`.
 */
template <conststr::charutils::char_like T, typename U, std::size_t N>
    requires conststr::meta::hashable<
        typename conststr::cstr<N, T, U>::view_type>
struct std::hash<conststr::cstr<N, T, U>> {
    std::size_t operator()(const conststr::cstr<N, T, U> &str) const noexcept {
        using view = typename conststr::cstr<N, T, U>::view_type;
        return std::hash<view>{}(static_cast<view>(str));
    }
};

#endif