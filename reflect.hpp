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
 * @file reflect.hpp
 * @brief Header file of comiple-time reflection support.
 */

#ifndef REFLECT_HPP
#define REFLECT_HPP

#include <functional>

#include "conststr.hpp"

/**
 * @brief Compile-time reflection for aggregate types.
 */
namespace reflect {
/**
 * @brief An object of `T` that can be used in constant context without really construct it.
 * @tparam T any type
 */
template <typename T>
extern const T fake_obj;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

/**
 * @brief Type that can be implicitly converted to any type.
 * @warning Can only used in unevaluated contexts.
 */
struct any_type {
    /**
     * @brief Convert to any type.
     * @tparam T any type
     * @return Object of any type.
     */
    template <typename T>
    [[maybe_unused]] constexpr operator T() const noexcept;
};

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

/**
 * @brief Internal implementation of `number_of_members`.
 * Get The number of a default-constructible aggregate type's members.
 * @details
 * Just call `number_of_members_impl<T>()` and keep the template pack `Args` empty.
 * On how it works, it first tests whether `T` can be constructed with one parameter,
 * if so, then tries to construct it with two parameters, and so on until `T` cannot
 * be constructed with `N` parameters, then the number of members of `T` is `N - 1`.
 * @tparam T must be a default-constructible aggregate type
 * @tparam Args for recursion only, keep it empty
 * @return The number of T's members.
 * @see number_of_members
 */
template <typename T, typename... Args>
consteval std::size_t number_of_members_impl()
    requires(std::is_aggregate_v<std::remove_cvref_t<T>> &&
             std::is_default_constructible_v<std::remove_cvref_t<T>>)
{
    using T_ = std::remove_cvref_t<T>;
    if constexpr (requires { T_{{Args{}}..., {any_type{}}}; })
        return number_of_members_impl<T_, Args..., any_type>();
    else if constexpr (requires { T_{{Args{}}..., {std::nullptr_t{}}}; })
        return number_of_members_impl<T_, Args..., std::nullptr_t>();
    else
        return sizeof...(Args);
}

/**
 * @brief The number of a default-constructible aggregate type's members.
 * @tparam T must be a default-constructible aggregate type
 */
template <typename T>
constexpr std::size_t number_of_members = number_of_members_impl<T>();

/**
 * @brief Convert a value of type `T` to a tuple containing references to all its members.
 * @note Only supports 128 members at most.
 * @tparam T any type.
 * @tparam N number of `T`'s members, automatically calculated if `T` is a default
 * constructible aggregate type. Otherwise, the caller needs to provide it.
 * @return Tuple containing references to all `t`'s members.
 */
template <class T, std::size_t N = number_of_members<T>>
constexpr decltype(auto) to_tuple(T &&t)
    requires(N <= 128)
{
    if constexpr (N == 0) {
        return std::tuple{};
    } else if constexpr (N == 1) {
        auto &&[p] = t;
        return std::tie(p);
    } else if constexpr (N == 2) {
        auto &&[p0, p1] = t;
        return std::tie(p0, p1);
    } else if constexpr (N == 3) {
        auto &&[p0, p1, p2] = t;
        return std::tie(p0, p1, p2);
    } else if constexpr (N == 4) {
        auto &&[p0, p1, p2, p3] = t;
        return std::tie(p0, p1, p2, p3);
    } else if constexpr (N == 5) {
        auto &&[p0, p1, p2, p3, p4] = t;
        return std::tie(p0, p1, p2, p3, p4);
    } else if constexpr (N == 6) {
        auto &&[p0, p1, p2, p3, p4, p5] = t;
        return std::tie(p0, p1, p2, p3, p4, p5);
    } else if constexpr (N == 7) {
        auto &&[p0, p1, p2, p3, p4, p5, p6] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6);
    } else if constexpr (N == 8) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7);
    } else if constexpr (N == 9) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8);
    } else if constexpr (N == 10) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
    } else if constexpr (N == 11) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    } else if constexpr (N == 12) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    } else if constexpr (N == 13) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    } else if constexpr (N == 14) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13);
    } else if constexpr (N == 15) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
                p14] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14);
    } else if constexpr (N == 16) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15);
    } else if constexpr (N == 17) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16);
    } else if constexpr (N == 18) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17);
    } else if constexpr (N == 19) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18);
    } else if constexpr (N == 20) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19);
    } else if constexpr (N == 21) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20);
    } else if constexpr (N == 22) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21);
    } else if constexpr (N == 23) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22);
    } else if constexpr (N == 24) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23);
    } else if constexpr (N == 25) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24);
    } else if constexpr (N == 26) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25);
    } else if constexpr (N == 27) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26);
    } else if constexpr (N == 28) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26,
                p27] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27);
    } else if constexpr (N == 29) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28);
    } else if constexpr (N == 30) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29);
    } else if constexpr (N == 31) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30);
    } else if constexpr (N == 32) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31);
    } else if constexpr (N == 33) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32);
    } else if constexpr (N == 34) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33);
    } else if constexpr (N == 35) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34);
    } else if constexpr (N == 36) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35);
    } else if constexpr (N == 37) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36);
    } else if constexpr (N == 38) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37);
    } else if constexpr (N == 39) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38);
    } else if constexpr (N == 40) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39);
    } else if constexpr (N == 41) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39,
                p40] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40);
    } else if constexpr (N == 42) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41);
    } else if constexpr (N == 43) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42);
    } else if constexpr (N == 44) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43);
    } else if constexpr (N == 45) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44);
    } else if constexpr (N == 46) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45);
    } else if constexpr (N == 47) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46);
    } else if constexpr (N == 48) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47);
    } else if constexpr (N == 49) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48);
    } else if constexpr (N == 50) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49);
    } else if constexpr (N == 51) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50);
    } else if constexpr (N == 52) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51);
    } else if constexpr (N == 53) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52);
    } else if constexpr (N == 54) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52,
                p53] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53);
    } else if constexpr (N == 55) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54);
    } else if constexpr (N == 56) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55);
    } else if constexpr (N == 57) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56);
    } else if constexpr (N == 58) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57);
    } else if constexpr (N == 59) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58);
    } else if constexpr (N == 60) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59);
    } else if constexpr (N == 61) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60);
    } else if constexpr (N == 62) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61);
    } else if constexpr (N == 63) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62);
    } else if constexpr (N == 64) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63);
    } else if constexpr (N == 65) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64);
    } else if constexpr (N == 66) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65);
    } else if constexpr (N == 67) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65,
                p66] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66);
    } else if constexpr (N == 68) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67);
    } else if constexpr (N == 69) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68);
    } else if constexpr (N == 70) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69);
    } else if constexpr (N == 71) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70);
    } else if constexpr (N == 72) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71);
    } else if constexpr (N == 73) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72);
    } else if constexpr (N == 74) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73);
    } else if constexpr (N == 75) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74);
    } else if constexpr (N == 76) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75);
    } else if constexpr (N == 77) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76);
    } else if constexpr (N == 78) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77);
    } else if constexpr (N == 79) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78);
    } else if constexpr (N == 80) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                p79] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79);
    } else if constexpr (N == 81) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80);
    } else if constexpr (N == 82) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81);
    } else if constexpr (N == 83) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82);
    } else if constexpr (N == 84) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83);
    } else if constexpr (N == 85) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84);
    } else if constexpr (N == 86) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85);
    } else if constexpr (N == 87) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86);
    } else if constexpr (N == 88) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87);
    } else if constexpr (N == 89) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88);
    } else if constexpr (N == 90) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89);
    } else if constexpr (N == 91) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90);
    } else if constexpr (N == 92) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91);
    } else if constexpr (N == 93) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91,
                p92] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92);
    } else if constexpr (N == 94) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93);
    } else if constexpr (N == 95) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94);
    } else if constexpr (N == 96) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95);
    } else if constexpr (N == 97) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96);
    } else if constexpr (N == 98) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97);
    } else if constexpr (N == 99) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98);
    } else if constexpr (N == 100) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99);
    } else if constexpr (N == 101) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100);
    } else if constexpr (N == 102) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101);
    } else if constexpr (N == 103) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102);
    } else if constexpr (N == 104) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103);
    } else if constexpr (N == 105) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103,
                p104] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104);
    } else if constexpr (N == 106) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105);
    } else if constexpr (N == 107) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105, p106);
    } else if constexpr (N == 108) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105, p106, p107);
    } else if constexpr (N == 109) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105, p106, p107, p108);
    } else if constexpr (N == 110) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105, p106, p107, p108, p109);
    } else if constexpr (N == 111) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110);
    } else if constexpr (N == 112) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111);
    } else if constexpr (N == 113) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112);
    } else if constexpr (N == 114) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113);
    } else if constexpr (N == 115) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114);
    } else if constexpr (N == 116) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115);
    } else if constexpr (N == 117) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105, p106, p107, p108, p109,
                        p110, p111, p112, p113, p114, p115, p116);
    } else if constexpr (N == 118) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105, p106, p107, p108, p109,
                        p110, p111, p112, p113, p114, p115, p116, p117);
    } else if constexpr (N == 119) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105, p106, p107, p108, p109,
                        p110, p111, p112, p113, p114, p115, p116, p117, p118);
    } else if constexpr (N == 120) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115,
            p116, p117, p118, p119);
    } else if constexpr (N == 121) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119, p120] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115,
            p116, p117, p118, p119, p120);
    } else if constexpr (N == 122) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119, p120, p121] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115,
            p116, p117, p118, p119, p120, p121);
    } else if constexpr (N == 123) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119, p120, p121, p122] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115,
            p116, p117, p118, p119, p120, p121, p122);
    } else if constexpr (N == 124) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119, p120, p121, p122, p123] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115,
            p116, p117, p118, p119, p120, p121, p122, p123);
    } else if constexpr (N == 125) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119, p120, p121, p122, p123, p124] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115,
            p116, p117, p118, p119, p120, p121, p122, p123, p124);
    } else if constexpr (N == 126) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119, p120, p121, p122, p123, p124,
                p125] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115,
            p116, p117, p118, p119, p120, p121, p122, p123, p124, p125);
    } else if constexpr (N == 127) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119, p120, p121, p122, p123, p124,
                p125, p126] = t;
        return std::tie(
            p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
            p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
            p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
            p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
            p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
            p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
            p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
            p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
            p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115,
            p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126);
    } else if constexpr (N == 128) {
        auto &&[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27,
                p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40,
                p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53,
                p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66,
                p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79,
                p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92,
                p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104,
                p105, p106, p107, p108, p109, p110, p111, p112, p113, p114,
                p115, p116, p117, p118, p119, p120, p121, p122, p123, p124,
                p125, p126, p127] = t;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                        p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23,
                        p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34,
                        p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45,
                        p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56,
                        p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67,
                        p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78,
                        p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89,
                        p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100,
                        p101, p102, p103, p104, p105, p106, p107, p108, p109,
                        p110, p111, p112, p113, p114, p115, p116, p117, p118,
                        p119, p120, p121, p122, p123, p124, p125, p126, p127);
    }
}

/**
 * @brief Wrapper of pointer type.
 * @details
 * We cannot directly pass a pointer of the member of an object as the
 * variable template parameter when we use clang as the compiler.
 * Therefore, we wrapper the pointer to a constexpr `cptr` object and pass it
 * as the variable template paramter.
 * @tparam T any type
 */
template <typename T>
struct cptr {
    const T *const ptr;
};

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

/**
 * @brief Type of N-th member of a default-constructible aggregate type `T`.
 * @tparam T any default-constructible aggregate type
 * @tparam N index of member
 */
template <typename T, std::size_t N>
using type_of = std::remove_cvref_t<
    std::tuple_element_t<N, decltype(to_tuple(fake_obj<T>))>>;

/**
 * @brief Get the pointer of the N-th member as a constexpr value.
 * @tparam T any type
 * @tparam N index of the member
 * @return `cptr` wrapped pointer.
 */
template <typename T, std::size_t N>
consteval auto cptr_of_member() {
    decltype(auto) member = std::get<N>(to_tuple(fake_obj<T>));
    return cptr<std::remove_cvref_t<decltype(member)>>{&member};
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

/**
 * @brief Core function to get member name via compiler built-in macro.
 * @note
 * When using GCC, will return a string in the form of
 * `"consteval auto reflect::pretty_name() [with cptr<...auto...> Ptr = ...]"`.
 * @note
 * When using clang, will return a string in the form of
 * `"auto reflect::pretty_name() [Ptr = ...]"`.
 * @note
 * When using MSVC, will return a string in the form of
 * `"auto __cdecl reflect::pretty_name<...>(void)"`.
 * @tparam Ptr pointer to the the variable or field you want to reflect
 * @return A compile-time string containing the name of the variable or field
 * and of type `conststr::cstr`.
 * @see conststr::cstr
 * @see name_of()
 */
template <cptr Ptr>
consteval auto pretty_name() {
#if defined(__clang__) || defined(__GNUC__)
    return conststr::cstr(__PRETTY_FUNCTION__);
#elif defined(_MSC_VER)
    return conststr::cstr(__FUNCSIG__);
#endif
}

/**
 * @brief Internal implementation of `basename_of`.
 * get the Base name of a full qualified name.
 * @tparam Name string containing a name that may be quailfied
 * @return Base name of a full qualified name.
 * @see basename_of
 */
template <conststr::cstr Name>
consteval auto basename_of_impl() {
    constexpr auto isident = [](const char &ch) {
        return conststr::charutils::isalnum(ch) || ch == '_';
    };
    constexpr std::size_t end = Name.rfind_if(isident) + 1;
    constexpr std::size_t begin = Name.rfind_if(std::not_fn(isident), end) + 1;
    return Name.template substr<begin, end - begin>();
}

/**
 * @brief Base name of a full qualified name.
 * @details
 * For example, `basename_of<"xxx::yyy::zzz->nnn"_cs>` is `"nnn"_cs`.
 * @tparam Name Name the name that may be quailfied.
 */
template <conststr::cstr Name>
constexpr auto basename_of = basename_of_impl<Name>();

/**
 * @brief Internal implementation of `name_of_ptr`.
 * Extract the real name from the output of `pretty_name()`.
 * @tparam Ptr pointer to the the variable or class member you want to reflect.
 * @return Name of the variable or class member.
 * @see name_of_ptr
 */
template <cptr Ptr>
consteval auto name_of_ptr_impl() {
    constexpr auto name = pretty_name<Ptr>();
#if defined(__clang__)
    constexpr auto prefix = conststr::cstr("{&");
    constexpr auto suffix = conststr::cstr("}]");
#elif defined(__GNUC__)
    constexpr auto prefix = conststr::cstr("{(& ");
    constexpr auto suffix = conststr::cstr(")}]");
#elif defined(_MSC_VER)
    constexpr auto prefix = conststr::cstr("reflect::fake_obj<");
    constexpr auto suffix = conststr::cstr("}>");
#endif
    constexpr auto begin = name.find(prefix) + prefix.size();
    constexpr auto end = name.rfind(suffix);
    constexpr auto path = name.template substr<begin, end - begin>();

    return basename_of<path>;
}

#ifdef _MSC_VER
/**
 * @brief Name of N-th member of a default-constructible aggregate type `T`.
 * @tparam T any default-constructible aggregate type
 * @tparam N index of member
 * @note Because MSVC will fail in some cases when compiling `name_of_ptr`,
 * therefore, a dedicated version is implemented for it.
 */
template <typename T, std::size_t N>
constexpr auto name_of = name_of_ptr_impl<cptr_of_member<T, N>()>();
#else
/**
 * @brief Get the variable or class member name from its pointer.
 * @tparam Ptr pointer to the the variable or class member you want to reflect.
 * @note MSVC currently fails to compile it when `Ptr` points to an array.
 */
template <cptr Ptr>
constexpr auto name_of_ptr = name_of_ptr_impl<Ptr>();

/**
 * @brief Name of N-th member of a default-constructible aggregate type `T`.
 * @tparam T any default-constructible aggregate type
 * @tparam N index of member
 */
template <typename T, std::size_t N>
constexpr auto name_of = name_of_ptr<cptr_of_member<T, N>()>;
#endif
}  // namespace reflect

#endif