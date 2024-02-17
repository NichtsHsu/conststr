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

    static_assert(reflect::name_of<MyStruct, 0> == "number"_cs);
    static_assert(reflect::name_of<MyStruct, 1> == "decimal"_cs);
    static_assert(reflect::name_of<MyStruct, 2> == "name"_cs);
    static_assert(reflect::name_of<MyStruct, 3> == "array"_cs);
    static_assert(reflect::name_of<MyStruct, 4> == "pointer"_cs);
    static_assert(reflect::name_of<MyStruct, 5> == "func_pointer"_cs);
    static_assert(reflect::name_of<MyStruct, 6> == "member_pointer"_cs);
    static_assert(reflect::name_of<MyStruct, 7> == "member_func_point"_cs);
    static_assert(reflect::name_of<MyStruct, 8> == "uncopyable"_cs);
    static_assert(reflect::name_of<MyStruct, 9> == "ref_wrapper"_cs);

    std::cout << __FILE__ ": all tests passed." << std::endl;

    return 0;
}
