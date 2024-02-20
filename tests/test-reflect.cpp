#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "reflect.hpp"

using conststr::cstr;
using namespace conststr::literal;

int value = 0;

struct helper {};

struct MyStruct {
    int number;
    double decimal;
    std::string name;
    std::size_t array[16] = {0};
    void *pointer;
    int (*func_pointer)(int);
    int helper::*member_pointer;
    int (helper::*member_func_point)(int);
    std::unique_ptr<int> uncopyable;
    std::reference_wrapper<int> ref_wrapper = value;
};

int main() {
    static_assert(reflect::number_of_members<MyStruct> == 10);

    static_assert(std::same_as<reflect::type_of<MyStruct, 0>, int>);
    static_assert(std::same_as<reflect::type_of<MyStruct, 1>, double>);
    static_assert(std::same_as<reflect::type_of<MyStruct, 2>, std::string>);
    static_assert(std::same_as<reflect::type_of<MyStruct, 3>, std::size_t[16]>);
    static_assert(std::same_as<reflect::type_of<MyStruct, 4>, void *>);
    static_assert(std::same_as<reflect::type_of<MyStruct, 5>, int (*)(int)>);
    static_assert(std::same_as<reflect::type_of<MyStruct, 6>, int helper::*>);
    static_assert(
        std::same_as<reflect::type_of<MyStruct, 7>, int (helper::*)(int)>);
    static_assert(
        std::same_as<reflect::type_of<MyStruct, 8>, std::unique_ptr<int>>);
    static_assert(std::same_as<reflect::type_of<MyStruct, 9>,
                               std::reference_wrapper<int>>);

    static_assert(reflect::name_of<MyStruct, 0> == "number");
    static_assert(reflect::name_of<MyStruct, 1> == "decimal");
    static_assert(reflect::name_of<MyStruct, 2> == "name");
    static_assert(reflect::name_of<MyStruct, 3> == "array");
    static_assert(reflect::name_of<MyStruct, 4> == "pointer");
    static_assert(reflect::name_of<MyStruct, 5> == "func_pointer");
    static_assert(reflect::name_of<MyStruct, 6> == "member_pointer");
    static_assert(reflect::name_of<MyStruct, 7> == "member_func_point");
    static_assert(reflect::name_of<MyStruct, 8> == "uncopyable");
    static_assert(reflect::name_of<MyStruct, 9> == "ref_wrapper");

    static_assert(
        std::same_as<reflect::type_of_member<MyStruct, "number">, int>);
    static_assert(
        std::same_as<reflect::type_of_member<MyStruct, "decimal">, double>);
    static_assert(
        std::same_as<reflect::type_of_member<MyStruct, "name">, std::string>);
    static_assert(std::same_as<reflect::type_of_member<MyStruct, "array">,
                               std::size_t[16]>);
    static_assert(
        std::same_as<reflect::type_of_member<MyStruct, "pointer">, void *>);
    static_assert(
        std::same_as<reflect::type_of_member<MyStruct, "func_pointer">,
                     int (*)(int)>);
    static_assert(
        std::same_as<reflect::type_of_member<MyStruct, "member_pointer">,
                     int helper::*>);
    static_assert(
        std::same_as<reflect::type_of_member<MyStruct, "member_func_point">,
                     int (helper::*)(int)>);
    static_assert(std::same_as<reflect::type_of_member<MyStruct, "uncopyable">,
                               std::unique_ptr<int>>);
    static_assert(std::same_as<reflect::type_of_member<MyStruct, "ref_wrapper">,
                               std::reference_wrapper<int>>);

    MyStruct s;
    decltype(auto) member0 = reflect::member_of<0>(s);
    member0 = 114;
    decltype(auto) member0_ = reflect::member_of<0>(std::move(s));
    static_assert(std::is_lvalue_reference_v<decltype(member0)>);
    static_assert(std::is_rvalue_reference_v<decltype(member0_)>);
    if (s.number != 114) return 1;
    decltype(auto) member3 = reflect::member_of<3>(s);
    member3[10] = 514;
    if (s.array[10] != 514) return 1;
    decltype(auto) member4 = reflect::member_of<"pointer">(s);
    decltype(auto) member4_ = reflect::member_of<"pointer">(std::move(s));
    static_assert(std::is_lvalue_reference_v<decltype(member4)>);
    static_assert(std::is_rvalue_reference_v<decltype(member4_)>);
    member4 = (void *)0x1919810;
    if (s.pointer != (void *)0x1919810) return 1;

    std::cout << __FILE__ ": all tests passed." << std::endl;

    return 0;
}
